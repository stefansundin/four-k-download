/// Copyright 2010-2012 4kdownload.com (developers@4kdownload.com)
/**
    This file is part of 4k Download.

    4k Download is free software; you can redistribute it and/or modify
    it under the terms of the one of two licenses as you choose:

    1. GNU GENERAL PUBLIC LICENSE Version 3
    (See file COPYING.GPLv3 for details).

    2. 4k Download Commercial License
    (Send request to developers@4kdownload.com for details).
   
*/



// precompiled header begin
#include "DTHeadersVideoDownload.h"
// precompiled header end

#include "DTDownloaderMetacafe.h"
#include "DTDownloaderUtils.h"
#include "DTMediaDownloaderDetails.h"
#include "DTSimpleMediaParser.h"
#include <openmedia/DTMediaDownloader.h>
#include <openmedia/DTString.h>
#include <set>

#include "DTMediaDownloaderDetails.h"

#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <boost/exception/diagnostic_information.hpp>

namespace openmedia { namespace downloader { namespace metacafe {

namespace {

bool get_property(const std::string & content, const char * propname, std::string & propValue)
{
    boost::regex re( std::string("\"") +  propname + "\"\\s*:\\s*\"([^\"]*)\"");
    boost::sregex_token_iterator i(content.begin(), content.end(), re, 1);
    boost::sregex_token_iterator j;
    if (i != j)
    {
        propValue = (*i).str();
        return true;
    } else
        return false;
}

//    <meta name="title" content="Mortal Kombat: New Fatalities Highlights">
std::string get_title(const std::string & content)
{
    boost::regex re("<meta[\\s]+name[\\s]*=[\\s]*\"title\"[\\s]+content[\\s]*=[\\s]*\"([^\"]*)\"");
    boost::sregex_token_iterator i(content.begin(), content.end(), re, 1);
    boost::sregex_token_iterator j;
    while(i != j)
    {
        return (*i++).str();
    }
    return "Unknown";
}

// <link rel="image_src" href="http://s1.mcstatic.com/thumb/6975268.jpg" />
std::string get_thumb_url(const std::string & content)
{
    boost::regex re("<link[\\s]+rel[\\s]*=[\\s]*\"image_src\"[\\s]+href[\\s]*=[\\s]*\"([^\"]*)\"");
    boost::sregex_token_iterator i(content.begin(), content.end(), re, 1);
    boost::sregex_token_iterator j;
    while(i != j)
    {
        return (*i++).str();
    }
    return "";
}


}

media_download_list_ptr parse_url(const std::string & Url)
{
    std::string main_headers, main_content, info_headers, info_content;
    if (!download_page(Url, main_headers, main_content, "", "Keep-Alive"))
        throw url_parser::exception();

    std::vector<HttpHeader> parsedHeaders;
    parse_http_headers(main_headers, parsedHeaders); 

    const std::string cookies = use_cookies(parsedHeaders);

    const std::string title = get_title(main_content);
    const std::wstring titleW = ::openmedia::utf8_to_utf16(title);

    std::string mediaData;
    get_property(main_content, "mediaData", mediaData);
    mediaData = url_decode(mediaData);
    
    std::string durationStr;
    get_property(main_content, "duration", durationStr);

    int duration = 0;
    try {
        duration = 1000 * boost::lexical_cast<int>(durationStr);
    }
    catch(...)
    {
    }

    media_download_list::BytesArrayPtr thumbnail;
    const std::string thumbUrl =  get_thumb_url(main_content);
    try 
    {
        std::string headers__, content__;
        if (download_page(thumbUrl, headers__, content__, "", ""))
        {
            thumbnail = media_download_list::BytesArrayPtr( new media_download_list::BytesArray( content__.size() ) );
            ::std::copy( content__.begin(), content__.end(), thumbnail->begin() );
        };
    }
    catch(...)
    {

    }
    
    std::string mp4Url, mp4HDUrl;

    ::std::stringstream mediaDataStrm;
    boost::algorithm::replace_all(mediaData, "\\/", "/");
    mediaDataStrm << mediaData;
    try 
    {
        boost::property_tree::ptree pt;
        boost::property_tree::json_parser::read_json(mediaDataStrm, pt);
        
        const std::string mp4Val = pt.get_optional<std::string>("MP4.mediaURL").get_value_or("");
        const std::string mp4Key = pt.get_optional<std::string>("MP4.key").get_value_or("");
        mp4Url = mp4Val + (mp4Key.size() ? (::std::string("?__gda__=") +  mp4Key) : "" );
        
        const std::string mp4HDVal = pt.get_optional<std::string>("highDefinitionMP4.mediaURL").get_value_or("");
        const std::string mp4HDKey = pt.get_optional<std::string>("highDefinitionMP4.key").get_value_or("");

        mp4HDUrl = mp4HDVal + (mp4HDKey.size() ? (::std::string("?__gda__=") +  mp4HDKey) : "" );

    }
    catch(boost::exception & e)
    {
    }


    std::vector< media_download_info > mediaInfoList;

    for (int i = 0; i < 2; ++i)
    {
        std::string videoUrl = (i==0) ? mp4Url : mp4HDUrl;
        if (videoUrl.length() <= 0)
            continue;

        std::string mediaHeaders/*, mediaContent*/;
        download_page_header(videoUrl, mediaHeaders, cookies, "keep-alive");
        //download_page(videoUrl, mediaHeaders, mediaContent, cookies, "keep-alive");
        std::vector<HttpHeader> parsedMediaHeaders;
        parse_http_headers(mediaHeaders, parsedMediaHeaders); 

        std::string mediaType;
        boost::uint64_t contentSize;
        if (is_media_content(parsedMediaHeaders, mediaType, contentSize))
        {
            media_url_handle_ptr urlHandle = media_url_handle_ptr( new media_url_handle(videoUrl, mediaType, contentSize, cookies, "") );

            const media_content_type_t mediaType_ = ::openmedia::downloader::parse_media_content_type(mediaType);
            const media_quality_type_t media_quality_type = ((i == 0) ? mediaQuality360P : mediaQuality720P);
            const media_video_type_t media_video_codec_type = mediaVideoH264;
            const media_audio_type_t media_audio_codec_type = mediaAudioAAC;

            int width_ = 0, height_ = 0;

            media_info_handle_ptr mediaInfoHandle 
                = media_info_handle_ptr( 
                new media_info_handle(
                width_, 
                height_, 
                media_quality_type,
                mediaType_,
                media_video_codec_type, 
                media_audio_codec_type, 
                duration) );

            mediaInfoList.push_back( media_download_info(urlHandle, mediaInfoHandle) );

        }
    }

    ::std::sort(mediaInfoList.begin(), mediaInfoList.end(), &compare_media_download_info);

    boost::shared_ptr<media_download_list::Impl> media_download_list_impl( new media_download_list::Impl(titleW, title, thumbnail, duration, Url) );
    media_download_list_impl->get_array().swap(mediaInfoList);

    return media_download_list_ptr( new media_download_list(media_download_list_impl) );
}

} } }

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


#include <openmedia/DTHeaders.h>

#include "DTDownloaderXnxx.h"
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

#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

namespace openmedia { namespace downloader { namespace xnxx {

std::string get_xnxx_flv_url(const std::string & content)
{
    boost::regex re( std::string("flv_url=(http[^&]*)&amp;") );
    boost::sregex_token_iterator i(content.begin(), content.end(), re, 1);
    boost::sregex_token_iterator j;
    if (i != j)
    {
        return (*i).str();
    } else
        return "";
}

std::string get_xnxx_thumb_url(const std::string & content)
{
    boost::regex re( std::string("url_bigthumb=(http[^&]*)&amp;") );
    boost::sregex_token_iterator i(content.begin(), content.end(), re, 1);
    boost::sregex_token_iterator j;
    if (i != j)
    {
        return (*i).str();
    } else
        return "";
}

std::string get_xnxx_duration(const std::string & content)
{
    boost::regex re( std::string("([\\d]+)\\s+min") );
    boost::sregex_token_iterator i(content.begin(), content.end(), re, 1);
    boost::sregex_token_iterator j;
    if (i != j)
    {
        return (*i).str();
    } else
        return "";
}

std::string get_xnxx_title(const std::string & content)
{
    boost::regex re( std::string("<title>\\s*([^<]*)\\s*-\\s*XNXX\\.COM<") );
    boost::sregex_token_iterator i(content.begin(), content.end(), re, 1);
    boost::sregex_token_iterator j;
    if (i != j)
    {
        return (*i).str();
    } else
        return "";
}

media_download_list_ptr parse_url(const std::string & Url)
{
    std::string main_headers, main_content, info_headers, info_content;
    if (!download_page(Url, main_headers, main_content, "", "Keep-Alive"))
        throw url_parser::exception();

    std::vector<HttpHeader> parsedHeaders;
    parse_http_headers(main_headers, parsedHeaders); 

    media_download_list::BytesArrayPtr thumbnail;

    const std::string cookies = use_cookies(parsedHeaders);

    std::string url = url_decode(get_xnxx_flv_url(main_content));

    std::string thumbUrl = url_decode(get_xnxx_thumb_url(main_content));

    std::string title = get_xnxx_title(main_content);
    replace_xml_esc(title);
    const std::wstring titleW = ::openmedia::utf8_to_utf16(title);

    int duration = 0;
    try {
        duration = 1000 * 60 * boost::lexical_cast<int>(get_xnxx_duration(main_content));
    }
    catch(...)
    {
    
    }

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
    
    std::string mediaHeaders;
    std::string simpleContent;

    const size_t probeBytes = 8000;
    download_bytes(url, mediaHeaders, simpleContent, cookies, "", probeBytes);
    std::vector<HttpHeader> parsedMediaHeaders;
    parse_http_headers(mediaHeaders, parsedMediaHeaders); 

    std::string mediaType;
    boost::uint64_t contentSize;

    std::vector< media_download_info > mediaInfoList;

    if (is_media_content(parsedMediaHeaders, mediaType, contentSize))
    {
        media_url_handle_ptr urlHandle = media_url_handle_ptr( new media_url_handle(url, mediaType, contentSize, cookies, "") );

        const media_content_type_t mediaType_ = ::openmedia::downloader::parse_media_content_type(mediaType);

        int width = 0, height = 0;
        if (::openmedia::downloader::mediaContentVideoFlv == mediaType_)
        {
            flv_video_size(simpleContent, width, height);
        } 
        else if (::openmedia::downloader::mediaContentVideoMP4 == mediaType_)
        {
            mp4_video_size(simpleContent, width, height);                
        } 

        media_quality_type_t media_quality_type = select_media_quality(height);
        media_video_type_t media_video_codec_type = mediaVideoUnknown;
        media_audio_type_t media_audio_codec_type = mediaAudioUnknown;

        media_info_handle_ptr mediaInfoHandle 
            = media_info_handle_ptr( 
            new media_info_handle(
            width, 
            height, 
            media_quality_type,
            mediaType_,
            media_video_codec_type, 
            media_audio_codec_type) );

        mediaInfoList.push_back( media_download_info(urlHandle, mediaInfoHandle) );
    }

    boost::shared_ptr<media_download_list::Impl> media_download_list_impl( new media_download_list::Impl(titleW, title, thumbnail, duration, Url) );
    media_download_list_impl->get_array().swap(mediaInfoList);

    return media_download_list_ptr( new media_download_list(media_download_list_impl) );

}

} } }

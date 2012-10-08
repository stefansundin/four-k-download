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

#include "DTDownloaderVimeo.h"
#include "DTDownloaderUtils.h"
#include "DTMediaDownloaderDetails.h"
#include "DTSimpleMediaParser.h"
#include <openmedia/DTMediaDownloader.h>
#include <openmedia/DTString.h>
#include <set>
#include <boost/exception/diagnostic_information.hpp>

#include "DTMediaDownloaderDetails.h"

#include <string>
#include <vector>
#include <iostream>

#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>


namespace openmedia { namespace downloader { namespace vimeo {


namespace {
template <class It>
std::string searchContent(It i1, It i2, const std::string & reg, int pos = 1)
{
    boost::match_results< std::string::const_iterator > res;
    const std::string resStr = 
        (boost::regex_search(i1, i2, res,  boost::regex(reg), boost::match_default)) ?
        res[pos].str() : "";
    return resStr;
}

std::string parse_clip_id(const std::string & content)
{
    return searchContent(
        content.begin(),
        content.end(),
        "<link.*?rel=.*?\"canonical\".*?href=\"\\/(\\d+)\""
        );
}

std::string parse_signature(const std::string & conten)
{
    return searchContent(conten.begin(), conten.end(),
        "\"signature\".*?:.*?\"([^\"]+)\"");
}

std::string parse_title(const std::string & conten)
{
    return searchContent(conten.begin(), conten.end(),
        "\"title\"[^:]*:[^\"]*\"([^\"]+)\"");
}

std::string parse_timestamp(const std::string & conten)
{
    return searchContent(conten.begin(), conten.end(),
        "\"timestamp\".*?:.*?([\\d]+)");
}

std::string parse_duration(const std::string & conten)
{
    return searchContent(conten.begin(), conten.end(),
        "\"duration\".*?:.*?([\\d]+)");
}

std::vector<std::string> parse_qualities(const std::string & conten)
{
    std::string str = searchContent(conten.begin(), conten.end(),
        "\"qualities\".*?:.*?\\[([^\\]]+)\\]");
    std::vector<std::string> q;
    boost::algorithm::split(q, str, boost::algorithm::is_any_of(","));
    BOOST_FOREACH(std::string & s, q) { boost::algorithm::trim_if(s, boost::algorithm::is_any_of("\"")); }
    return q;
}


std::string get_clip_id_from_url(const std::string & Url)
{
    boost::regex re( std::string("[^\\d]+([\\d]+)") );
    boost::sregex_token_iterator i(Url.begin(), Url.end(), re, 1);
    boost::sregex_token_iterator j;
    if (i != j)
    {
        std::string id = (*i).str();
        return id;    
    }
    else
        return "";
}

std::string parse_thumbnail(const std::string & conten)
{
    std::string url = searchContent(conten.begin(), conten.end(),
        "\"thumbnail\".*?:.*?\"([^\"]+)\"");
    boost::algorithm::replace_all(url, "\\/", "/");
    return url;
}

}


media_download_list_ptr parse_url_impl(const std::string & Url, bool fromUrl);

media_download_list_ptr parse_url(const std::string & Url)
{
    media_download_list_ptr downloadList;
    try 
    {
        downloadList = parse_url_impl(Url, true);
        if (downloadList->size() > 0)
            return downloadList;
    }
    catch(...)
    {
    }
    return parse_url_impl(Url, false);
}


media_download_list_ptr parse_url_impl(const std::string & Url, bool fromUrl)
{
    const std::string clipIdUrl = (fromUrl ?  get_clip_id_from_url(Url) : "" );

    std::string main_headers, main_content;
    if (!download_page(Url, main_headers, main_content, "", "Keep-Alive"))
        throw url_parser::exception();

    std::string cookies;

    {
        std::vector<HttpHeader> parsedHeaders;
        parse_http_headers(main_headers, parsedHeaders); 
        cookies = use_cookies(parsedHeaders);
    }   

    const std::string clipId = (clipIdUrl == "") ? (parse_clip_id(main_content)) : (clipIdUrl);
    const std::string signature = parse_signature(main_content);
    const std::string timestamp = parse_timestamp(main_content);
    std::vector<std::string> q = parse_qualities(main_content);
    const std::string thumbUrl = parse_thumbnail(main_content);
    const std::string durationStr = parse_duration(main_content);
    const std::string titleUtf8 = parse_title(main_content);
    const std::wstring titleW = utf8_to_utf16(titleUtf8);

    media_download_list::BytesArrayPtr thumbnail;
    try 
    {
        std::string headers__, content__;
        if ((thumbUrl != "") && download_page(thumbUrl, headers__, content__, "", ""))
        {
            thumbnail = media_download_list::BytesArrayPtr( new media_download_list::BytesArray( content__.size() ) );
            ::std::copy( content__.begin(), content__.end(), thumbnail->begin() );
        };
    }
    catch(...)
    {
    }   
   
    if (clipId.empty() || 
        signature.empty() ||
        timestamp.empty() )
        url_parser::exception();

    int duration = 0;
    try
    {
        duration = boost::lexical_cast<int>( durationStr ) * 1000;
    } 
    catch(...)
    {
    }
    
    std::vector< media_download_info > mediaInfoList;

    BOOST_FOREACH(const std::string &s, q)
    {

        const std::string query = std::string("http://player.vimeo.com/play_redirect?") + 
            "clip_id=" + clipId +
            "&sig=" + signature + 
            "&time=" + timestamp +
            "&quality=" + s;
        
        std::string mediaHeaders;
        download_page_header(query, mediaHeaders, cookies, "");

        std::vector<HttpHeader> parsedMediaHeaders;
        parse_http_headers(mediaHeaders, parsedMediaHeaders); 

        std::string mediaType;
        boost::uint64_t contentSize;
        if (is_media_content(parsedMediaHeaders, mediaType, contentSize))
        {
             media_url_handle_ptr urlHandle = media_url_handle_ptr( new media_url_handle(query, mediaType, contentSize, cookies, "") );
             
             const media_content_type_t mediaType_ = ::openmedia::downloader::parse_media_content_type(mediaType);
             
             media_quality_type_t media_quality_type;
             if (s == "hd")
                 media_quality_type = mediaQuality720P;
             else if (s == "sd")
                 media_quality_type = mediaQuality360P;
             else if (s == "mobile")
                 media_quality_type = mediaQuality240P;
             else
                 continue;

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
    
    boost::shared_ptr<media_download_list::Impl> media_download_list_impl( new media_download_list::Impl(titleW, titleUtf8, thumbnail, duration, Url) );
    media_download_list_impl->get_array().swap(mediaInfoList);

    return media_download_list_ptr( new media_download_list(media_download_list_impl) );

}

} } } // namespace openmedia::downloader::vimeo

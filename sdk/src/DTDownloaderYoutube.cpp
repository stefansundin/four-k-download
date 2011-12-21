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

#include "DTDownloaderYoutube.h"
#include "DTDownloaderYoutubeDetails.h"
#include "DTDownloaderUtils.h"
#include "DTMediaDownloaderDetails.h"
#include "DTSimpleMediaParser.h"
#include <openmedia/DTMediaDownloader.h>
#include <openmedia/DTString.h>
#include <set>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/foreach.hpp>
#include <boost/timer.hpp>

#include "DTMediaDownloaderDetails.h"
#include "DTDownloaderYoutubeTools.h"

#include <string>
#include <vector>
#include <iostream>

namespace openmedia { namespace downloader { namespace youtube {

const char * keepAliveParam = "";

media_download_list::BytesArrayPtr download_thumbnail_impl(const std::string & ThumbnailUrl)
{
    if (!ThumbnailUrl.empty())
    {
        try 
        {
            std::string headers__, content__;
            if (download_page(ThumbnailUrl, headers__, content__, "", ""))
            {
                media_download_list::BytesArrayPtr thumbnail = media_download_list::BytesArrayPtr( new media_download_list::BytesArray( content__.size() ) );
                ::std::copy( content__.begin(), content__.end(), thumbnail->begin() );
                return thumbnail;
            };
        }
        catch(...)
        {
        }
    }
    return media_download_list::BytesArrayPtr();
}

media_download_list::BytesArrayPtr extract_thumbnail(const std::string & content)
{
    return download_thumbnail_impl( extract_youtube_thumbnail_url(content) );
}

boost::shared_ptr<media_download_list::Impl> parse_url_impl1(const std::string & Url)
{
    std::string main_headers, main_content, info_headers, info_content;
    //std::string normalizedUrl = normalize_youtube_url(Url);
    if (!download_page(Url, main_headers, main_content, "", keepAliveParam))
        throw url_parser::exception();

    std::vector<HttpHeader> parsedHeaders;
    parse_http_headers(main_headers, parsedHeaders); 

    const std::string cookies = use_cookies(parsedHeaders);

    std::string title ="Unknown";
    extract_youtube_title(main_content, title);
    const std::wstring titleW = ::openmedia::utf8_to_utf16(title);

    const std::string token = get_youtube_token(main_content);
    const std::string video_id = get_youtube_video_id(main_content);
    const boost::uint32_t duration = 1000 * get_youtube_duration(main_content);
    const std::string eurl = "http://www.youtube.com/watch?v=" + video_id;
    media_download_list::BytesArrayPtr thumbnail = extract_thumbnail(main_content);

    
    const std::string get_video_info = "http://www.youtube.com/get_video_info?video_id=" + url_encode(video_id) +
        "&el=$el" +
//        "&ps=default" +
//        "&eurl=" + url_encode(eurl)+
//        "&hl=en_US" +
        "&t=" + url_encode(token);

#if 0
    std::cout << "get_video_info url: " << get_video_info << "\n";
#endif

    if (!download_page(get_video_info, info_headers, info_content, cookies, ""))
        throw url_parser::exception();

    //const std::string info_content_decoded = url_decode(info_content);

    std::vector<std::string> urls;
    std::vector<int> ids;

    std::string decoded_info_content = url_decode(info_content);
    split_youtube_video_info(info_content, urls, ids);

    std::vector< media_download_info > mediaInfoList;

    ::std::set< boost::uint64_t > contentSizesSet;
       

    for (size_t i = 0; i < urls.size(); ++i)
    {
        const std::string url = url_decode(urls[i]);

#if 0
        std::cout << "\n\nURL: " << url << "\n";
#endif
        const size_t probeBytes = 8000;
        std::string mediaHeaders;
        std::string simpleContent;

        download_page_header(url, mediaHeaders, cookies, "");
        
        //download_bytes(url, mediaHeaders, simpleContent, cookies, "", probeBytes);

        std::vector<HttpHeader> parsedMediaHeaders;
        parse_http_headers(mediaHeaders, parsedMediaHeaders); 
        
        std::string mediaType;
        boost::uint64_t contentSize;
        if (is_media_content(parsedMediaHeaders, mediaType, contentSize))
        {
            if (!contentSizesSet.count(contentSize))
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
                else if (::openmedia::downloader::mediaContentVideoWebm == mediaType_)
                {
                    webm_video_size(simpleContent, width, height);                
                }

                media_quality_type_t media_quality_type;
                media_content_type_t mediaType2_;
                media_video_type_t media_video_codec_type;
                media_audio_type_t media_audio_codec_type;

                youtube_format_map(ids[i], 
                    media_quality_type, 
                    mediaType2_,
                    media_video_codec_type, 
                    media_audio_codec_type); 

                media_info_handle_ptr mediaInfoHandle 
                    = media_info_handle_ptr( 
                    new media_info_handle(
                    width, 
                    height, 
                    media_quality_type,
                    mediaType_,
                    media_video_codec_type,
                    media_audio_codec_type,
                    duration) );

                mediaInfoList.push_back( media_download_info(urlHandle, mediaInfoHandle) );
                contentSizesSet.insert(contentSize);
            }
        }
    }

    ::std::sort(mediaInfoList.begin(), mediaInfoList.end(), &compare_media_download_info);

    boost::shared_ptr<media_download_list::Impl> media_download_list_impl( new media_download_list::Impl(titleW, title, thumbnail, duration, Url) );
    media_download_list_impl->get_array().swap(mediaInfoList);
    
    return media_download_list_impl;
}

boost::shared_ptr<media_download_list::Impl> parse_url_impl2(const std::string & Url)
{
    std::string main_headers, main_content;
    
    if (!download_page(Url, main_headers, main_content, "", keepAliveParam))
        throw url_parser::exception();

    std::string value;
    if (search_watch_url(main_content, value) )
    {
        const std::string newUrl = std::string("http://www.youtube.com/watch?v=") + value;
        return parse_url_impl1(newUrl);        
    }
    else
        throw url_parser::exception();
}

bool is_embedded(const std::string & Url)
{
    return boost::algorithm::contains(Url, "/embed/") && (! boost::algorithm::contains(Url, "player_embedded"));
}

boost::shared_ptr<media_download_list::Impl> parse_url_embedded(const std::string & Url)
{
    std::string main_headers, main_content;
    if (!download_page(Url, main_headers, main_content, "", ""))
        throw url_parser::exception();

    std::vector<HttpHeader> parsedHeaders;
    parse_http_headers(main_headers, parsedHeaders); 
    const std::string cookies = use_cookies(parsedHeaders);

    std::string title = "Unknown";
    get_youtube_property(main_content, "title", title);
    const std::wstring titleW = ::openmedia::utf8_to_utf16(title);
    const boost::uint32_t duration = 1000 * get_youtube_duration(main_content);
    media_download_list::BytesArrayPtr thumbnail = extract_thumbnail(main_content);
    const std::string video_id = get_youtube_video_id(main_content);

    const std::string get_video_info = "http://www.youtube.com/get_video_info?video_id=" + url_encode(video_id) +
        "&hl=en_US" +
        "&eurl=unknown"+
        "&el=embedded";

    std::string info_headers, info_content;
    if (!download_page(get_video_info, info_headers, info_content, cookies, ""))
        throw url_parser::exception();

    //const std::string info_content_decoded = url_decode(info_content);
    std::vector<std::string> urls;
    std::vector<int> ids;
    split_youtube_video_info(info_content, urls, ids);

    ////////////////////////////////

    std::vector< media_download_info > mediaInfoList;
    ::std::set< boost::uint64_t > contentSizesSet;

    for (size_t i = 0; i < urls.size(); ++i)
    {
        const std::string url = url_decode(urls[i]);

#if 0
        std::cout << "\n\nURL: " << url << "\n";
#endif
        std::string mediaHeaders;
        std::string simpleContent;

        download_page_header(url, mediaHeaders, cookies, "");
        //download_bytes(url, mediaHeaders, simpleContent, cookies, "", probeBytes);

        std::vector<HttpHeader> parsedMediaHeaders;
        parse_http_headers(mediaHeaders, parsedMediaHeaders); 
        
        std::string mediaType;
        boost::uint64_t contentSize;
        if (is_media_content(parsedMediaHeaders, mediaType, contentSize))
        {
            if (!contentSizesSet.count(contentSize))
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
                else if (::openmedia::downloader::mediaContentVideoWebm == mediaType_)
                {
                    webm_video_size(simpleContent, width, height);                
                }

                media_quality_type_t media_quality_type;
                media_content_type_t mediaType2_;
                media_video_type_t media_video_codec_type;
                media_audio_type_t media_audio_codec_type;

                youtube_format_map(ids[i],
                    media_quality_type,
                    mediaType2_,
                    media_video_codec_type,
                    media_audio_codec_type);

                media_info_handle_ptr mediaInfoHandle
                    = media_info_handle_ptr(
                    new media_info_handle(
                    width,
                    height,
                    media_quality_type,
                    mediaType_,
                    media_video_codec_type,
                    media_audio_codec_type,
                    duration) );

                mediaInfoList.push_back( media_download_info(urlHandle, mediaInfoHandle) );
                contentSizesSet.insert(contentSize);
            }
        }
    }

    ::std::sort(mediaInfoList.begin(), mediaInfoList.end(), &compare_media_download_info);

    boost::shared_ptr<media_download_list::Impl> media_download_list_impl( new media_download_list::Impl(titleW, title, thumbnail, duration, Url) );
    media_download_list_impl->get_array().swap(mediaInfoList);
    
    return media_download_list_impl;
}

boost::shared_ptr<media_download_list::Impl> parse_url_impl3(const std::string & Url)
{
    std::string main_headers, main_content;
    if (!download_page(Url, main_headers, main_content, "", keepAliveParam))
        throw url_parser::exception();

    std::string value;
    if (search_watch_url(main_content, value) )
    {
        const std::string newUrl = std::string("http://www.youtube.com/embed/") + value;
        return parse_url_embedded(newUrl);        
    }
    else
        throw url_parser::exception();
}

bool pre_parse_playlist(const std::string & Url, std::vector<media_download_list_ptr> & ArrayOfVideos);

boost::shared_ptr<media_download_list::Impl> parse_url_get_impl(const std::string & Url)
{
    if (is_embedded(Url))
    {
        return parse_url_embedded(Url);
    }

    boost::shared_ptr<media_download_list::Impl> resultList;
    bool res = true;
    try
    {
        resultList = parse_url_impl1(Url);
    }
    catch(...)
    {
        res = false;
    }

    if (!(!res || !resultList || !resultList->size()))
        return resultList;

    res = true;
    try {
        resultList = parse_url_impl2(Url);
    }catch(...)
    {
        res = false;
    }

    if (!(!res || !resultList || !resultList->size()))
        return resultList;

    return parse_url_impl3(Url);
}

media_download_list_ptr parse_url(const std::string & Url)
{
    return media_download_list_ptr( new media_download_list(parse_url_get_impl(Url) ) );
}

std::vector<media_download_list_ptr> parse_url_playlist(const std::string & Url)
{
    std::vector<media_download_list_ptr> result;
    pre_parse_playlist(Url, result);
    return result;
}


media_download_list_ptr pre_parse_url(const std::string & Url)
{
    std::string main_headers, main_content;

    if (!download_page(Url, main_headers, main_content, "", ""))
        throw url_parser::exception();

    std::vector<HttpHeader> parsedHeaders;
    parse_http_headers(main_headers, parsedHeaders); 
    const std::string cookies = use_cookies(parsedHeaders);

    std::string title = "Unknown";
    extract_youtube_title(main_content, title);
    const std::wstring titleW = ::openmedia::utf8_to_utf16(title);
    const boost::uint32_t duration = 1000 * get_youtube_duration(main_content);

    media_download_list::BytesArrayPtr thumbnail = extract_thumbnail(main_content);

    boost::shared_ptr<media_download_list::Impl> 
        media_download_list_impl( new media_download_list::Impl(titleW, title, thumbnail, duration, Url) );
    
    return media_download_list_ptr( new media_download_list(media_download_list_impl) );
}

bool pre_parse_playlist(const std::string & Url, std::vector<media_download_list_ptr> & ArrayOfVideos)
{
    ArrayOfVideos.clear();

    std::vector<media_download_list_ptr> result; 

    std::string main_headers, main_content;
    if (!download_page(Url, main_headers, main_content, "", ""))
        throw url_parser::exception();

    std::vector< std::string > urlsArray;
    if (!parse_playlist_ids(main_content, urlsArray))
    {
        std::cerr << "[openmedia::downloader::youtube::pre_parse_playlist] : parse_playlist_ids failed\n";
        return false;
    }

    std::vector< youtube_desc > descArray;
    if (receive_youtube_info(userAgent, urlsArray, descArray))
    {
        BOOST_FOREACH( const youtube_desc & desc, descArray)
        {
            boost::shared_ptr<media_download_list::Impl> 
                media_download_list_impl( new media_download_list::Impl(
                desc.title(),
                desc.title_utf8(),
                media_download_list::BytesArrayPtr(), 
                0.0, 
                std::string("http://www.youtube.com/watch?v=") + desc.id()
                )
                );
            result.push_back( media_download_list_ptr(new media_download_list(media_download_list_impl, false) ) );
        }
    }
    else
    {
        std::cerr << "[openmedia::downloader::youtube::pre_parse_playlist] : receive_youtube_info failed\n";
    }

    ArrayOfVideos.swap(result);
    return true;
}

} } }

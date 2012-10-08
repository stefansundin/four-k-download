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


/// \file   DTDownloaderYoutube.cpp

#include "DTHeadersVideoDownload.h"

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

#include "videodownload/DTSubtitleYoutube.h"

using namespace std;

namespace openmedia { namespace downloader { namespace youtube {

const char * keepAliveParam = "";

media_download_list::BytesArrayPtr download_thumbnail_impl(const std::string & ThumbnailUrl)
{
    if (!ThumbnailUrl.empty())
    {
        try 
        {
            std::string headers, content;
            if (download_page(ThumbnailUrl, headers, content, "", ""))
            {
                media_download_list::BytesArrayPtr thumbnail 
                    = media_download_list::BytesArrayPtr( new media_download_list::BytesArray(content.size()) );
                std::copy( content.begin(), content.end(), thumbnail->begin() );
                return thumbnail;
            };
        }
        catch(...)
        {
        }
    }
    return media_download_list::BytesArrayPtr();
}

media_download_list::BytesArrayPtr extract_thumbnail(const string & content)
{
    return download_thumbnail_impl( extract_youtube_thumbnail_url(content) );
}

void parse_formats_list(
    const std::vector<string> &urls,
    const std::vector<int> &ids,
    const std::string& cookies, 
    int duration,
    std::vector<media_download_info> & resultMediaInfoList)
{
    std::set<boost::uint64_t> content_size_set;

    for (size_t i = 0; i < urls.size(); ++i)
    {
        string url = url_decode(urls[i]);


        std::string media_headers;
        if (!download_page_header(url, media_headers, cookies, ""))
            continue;       

        std::vector<HttpHeader> parsed_headers;
        parse_http_headers(media_headers, parsed_headers); 

        std::string media_type_str;
        boost::uint64_t content_size;
        if (is_media_content(parsed_headers, media_type_str, content_size))
        {
            if (!content_size_set.count(content_size))
            {
                media_url_handle_ptr urlHandle =
                    media_url_handle_ptr( new media_url_handle(url, media_type_str, content_size, cookies, "") );

                const media_content_type_t media_type = parse_media_content_type(media_type_str);

                media_quality_type_t media_quality_type;
                media_content_type_t media_type_2_;
                media_video_type_t media_video_codec_type;
                media_audio_type_t media_audio_codec_type;

                youtube_format_map(ids[i], 
                    media_quality_type, 
                    media_type_2_,
                    media_video_codec_type, 
                    media_audio_codec_type); 

                media_info_handle_ptr mediaInfoHandle 
                    = media_info_handle_ptr( 
                    new media_info_handle(
                    0, 0, 
                    media_quality_type,
                    media_type,
                    media_video_codec_type,
                    media_audio_codec_type,
                    duration) );

                resultMediaInfoList.push_back( media_download_info(urlHandle, mediaInfoHandle) );
                content_size_set.insert(content_size);
            }
        }
    }

    std::sort(resultMediaInfoList.begin(), resultMediaInfoList.end(), &compare_media_download_info);
}

boost::shared_ptr<media_download_list::Impl> parse_url_impl1(const std::string & Url)
{
    std::string video_id, token;

    {
        std::string headers, content;
        if (!download_page(Url, headers, content, "", ""))
            throw url_parser::exception();
        
        video_id = get_youtube_video_id(content);
        token = get_youtube_token(content);
    }

    std::string cookies;
    std::string info_content;

    {
        //const string video_info_url = "http://www.youtube.com/get_video_info?video_id=" + url_encode(video_id) +
        //    "&hl=en&gl=US&ptk=vevo&el=detailpage";  

        const string video_info_url = "http://www.youtube.com/get_video_info?video_id=" + url_encode(video_id) +
            "&el=$el" +
            //        "&ps=default" +
            //        "&eurl=" + url_encode(eurl)+
            //        "&hl=en_US" +
            "&t=" + url_encode(token);

        std::string headers, &content = info_content;
        if (!download_page(video_info_url, headers, content, "", ""))
            throw url_parser::exception();
        vector<HttpHeader> parsed_headers;
        parse_http_headers(headers, parsed_headers); 
        cookies = use_cookies(parsed_headers);
    }

    std::string title = "Unknown";
    std::wstring titleW;
    boost::uint32_t duration = 0;
    media_download_list::BytesArrayPtr thumbnail;
    subtitles_ptr subs;

    {
        std::string headers, content;
        if (!download_page(Url, headers, content, cookies, ""))
            throw url_parser::exception();

        vector<HttpHeader> parsed_headers;
        parse_http_headers(headers, parsed_headers);     
        std::string new_cookies = use_cookies(parsed_headers);    
        if (!new_cookies.empty())
            cookies += "; " + new_cookies;

        extract_youtube_title(content, title);
        titleW = ::openmedia::utf8_to_utf16(title);
        
        duration = 1000 * get_youtube_duration(content);                
        thumbnail = extract_thumbnail(content);

        std::string transcribed_url = get_transcribed_subtitles(content);
        subs = subtitles_youtube::create(video_id, transcribed_url);
    }    

    std::vector<string> urls;
    std::vector<int> ids;
    split_youtube_video_info(info_content, urls, ids);

    std::vector<media_download_info> media_info_list;
    parse_formats_list(urls, ids, cookies, duration, media_info_list);
    
    boost::shared_ptr<media_download_list::Impl> media_download_list_impl(
        new media_download_list::Impl(titleW, title, thumbnail, duration, Url, subs) 
        );

    media_download_list_impl->get_array().swap(media_info_list);

    return media_download_list_impl;
}

boost::shared_ptr<media_download_list::Impl> parse_url_impl2(const std::string & Url)
{
    std::string headres, content;
    
    if (!download_page(Url, headres, content, "", ""))
        throw url_parser::exception();

    std::string value;
    if (search_watch_url(content, value) )
    {
        const string newUrl = string("http://www.youtube.com/watch?v=") + value;
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
    std::string headers, content;
    if (!download_page(Url, headers, content, "", ""))
        throw url_parser::exception();

    std::vector<HttpHeader> parsedHeaders;
    parse_http_headers(headers, parsedHeaders); 
    const std::string cookies = use_cookies(parsedHeaders);

    std::string title = "Unknown";
    get_youtube_property(content, "title", title);
    const std::wstring titleW = ::openmedia::utf8_to_utf16(title);
    
    const boost::uint32_t duration = 1000 * get_youtube_duration(content);    

    media_download_list::BytesArrayPtr thumbnail = extract_thumbnail(content);
    const std::string video_id = get_youtube_video_id(content);
    
    const std::string transcribed_url = get_transcribed_subtitles(content);
    subtitles_ptr subs = subtitles_youtube::create(video_id, transcribed_url);

    const std::string get_video_info = 
        "http://www.youtube.com/get_video_info?video_id=" + 
        url_encode(video_id) +
        "&hl=en_US" +
        "&eurl=unknown"+
        "&el=embedded";

    std::string info_headers, info_content;
    if (!download_page(get_video_info, info_headers, info_content, cookies, ""))
        throw url_parser::exception();

    std::vector<string> urls;
    std::vector<int> ids;
    split_youtube_video_info(info_content, urls, ids);

    ////////////////////////////////

    std::vector<media_download_info> media_info_list;
    parse_formats_list(urls, ids, cookies, duration, media_info_list);

    boost::shared_ptr<media_download_list::Impl> media_download_list_impl( 
        new media_download_list::Impl(titleW, title, thumbnail, duration, Url, subs) );
    media_download_list_impl->get_array().swap(media_info_list);
    
    return media_download_list_impl;
}

boost::shared_ptr<media_download_list::Impl> parse_url_impl3(const string & Url)
{
    std::string headers, content;
    if (!download_page(Url, headers, content, "", keepAliveParam))
        throw url_parser::exception();

    std::string value;
    if (search_watch_url(content, value) )
    {
        const string newUrl = string("http://www.youtube.com/embed/") + value;
        return parse_url_embedded(newUrl);        
    }
    else
        throw url_parser::exception();
}

bool pre_parse_playlist(const string & Url, vector<media_download_list_ptr> & ArrayOfVideos);

boost::shared_ptr<media_download_list::Impl> parse_url_get_impl(const string & Url)
{
    if (is_embedded(Url))
        return parse_url_embedded(Url);

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
    try 
    {
        resultList = parse_url_impl2(Url);
    }
    catch(...)
    {
        res = false;
    }

    if (!(!res || !resultList || !resultList->size()))
        return resultList;

    return parse_url_impl3(Url);
}

media_download_list_ptr parse_url(const string & Url)
{
    return media_download_list_ptr( new media_download_list(parse_url_get_impl(Url) ) );
}

vector<media_download_list_ptr> parse_url_playlist(const string & Url)
{
    vector<media_download_list_ptr> result;
    pre_parse_playlist(Url, result);
    return result;
}

media_download_list_ptr pre_parse_url(const string & Url)
{
    string headers, content;

    if (!download_page(Url, headers, content, "", ""))
        throw url_parser::exception();

    vector<HttpHeader> parsedHeaders;
    parse_http_headers(headers, parsedHeaders); 
    const std::string cookies = use_cookies(parsedHeaders);

    std::string title = "Unknown";
    extract_youtube_title(content, title);
    const std::wstring titleW = ::openmedia::utf8_to_utf16(title);
    const boost::uint32_t duration = 1000 * get_youtube_duration(content);
    
    const string video_id = get_youtube_video_id(content);
    
    const std::string transcribed_url = get_transcribed_subtitles(content);
    subtitles_ptr subs = subtitles_youtube::create(video_id, transcribed_url);

    media_download_list::BytesArrayPtr thumbnail = extract_thumbnail(content);

    boost::shared_ptr<media_download_list::Impl> 
        media_download_list_impl( new media_download_list::Impl(titleW, title, thumbnail, duration, Url, subs) );
    
    return media_download_list_ptr( new media_download_list(media_download_list_impl) );
}

bool pre_parse_playlist(const string & Url, vector<media_download_list_ptr> & ArrayOfVideos)
{
    ArrayOfVideos.clear();

    std::vector<media_download_list_ptr> result; 

    std::string headers, content;
    if (!download_page(Url, headers, content, "", ""))
        throw url_parser::exception();

    std::vector<std::string> urlsArray;
    if (!parse_playlist_ids(content, urlsArray))
    {
        return false;
    }

    std::vector<youtube_desc> descArray;
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
                string("http://www.youtube.com/watch?v=") + desc.id(),
                subtitles_youtube::create(desc.id(), "")
                )
                );
            result.push_back( media_download_list_ptr(new media_download_list(media_download_list_impl, false) ) );
        }
    }
    else
    {
    }

    ArrayOfVideos.swap(result);
    return true;
}

} } }

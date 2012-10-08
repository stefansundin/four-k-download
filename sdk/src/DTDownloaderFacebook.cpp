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

#include "DTDownloaderFacebook.h"
#include "DTDownloaderUtils.h"
#include "DTMediaDownloaderDetails.h"
#include "DTSimpleMediaParser.h"
#include <openmedia/DTMediaDownloader.h>
#include <openmedia/DTString.h>
#include <set>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/replace.hpp>

#include "DTMediaDownloaderDetails.h"

#include <string>
#include <vector>
#include <iostream>

#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>


namespace openmedia { namespace downloader { namespace facebook {

std::string extract_prop(const std::string & Seq, const std::string & Prop)
{
    boost::regex re( std::string("\"") + Prop + "\"\\s*,\\s*\"([^\"]*)\"");
    boost::sregex_token_iterator i(Seq.begin(), Seq.end(), re, 1);
    boost::sregex_token_iterator j;
    if (i != j)
    {
        const std::string title = (*i).str();
        return title;    
    }
    else
        return "";

}


media_download_list_ptr parse_url(const std::string & Url)
{
    std::string UrlHttp = Url;
    boost::algorithm::replace_all(UrlHttp, "https://", "http://");
    std::string main_headers, main_content;
    if (!download_page(UrlHttp, main_headers, main_content, "", ""))
        throw url_parser::exception();

    std::vector<HttpHeader> parsedHeaders;
    parse_http_headers(main_headers, parsedHeaders); 
    const std::string cookies = use_cookies(parsedHeaders);

    std::string title = replace_u_codes_6(extract_prop(main_content, "video_title"));
    boost::algorithm::replace_all(title, "+", " ");

    const std::wstring titleW = ::openmedia::utf8_to_utf16(title);
    const std::string titleUtf8 = utf16_to_utf8(titleW);

    const std::string durationStr = extract_prop(main_content, "video_length");

    int duration = 0;
    try
    {
        duration = boost::lexical_cast<int>( durationStr );
    } 
    catch(...)
    {
    }

    const std::string thumbUrl = replace_u_codes_6(extract_prop(main_content, "thumb_url"));

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

    std::vector< media_download_info > mediaInfoList;


    for (int i = 0; i < 2; ++i)
    {
        std::string videoUrl = replace_u_codes_6(extract_prop(main_content, ((i==0)?"lowqual_src":"highqual_src")));

        if (videoUrl.size() > 0)
        {
            std::string mediaHeaders/*, mediaContent*/;
            download_page_header(videoUrl, mediaHeaders, cookies, "keep-alive");
            std::vector<HttpHeader> parsedMediaHeaders;
            parse_http_headers(mediaHeaders, parsedMediaHeaders); 

            std::string mediaType;
            boost::uint64_t contentSize;
            if (is_media_content(parsedMediaHeaders, mediaType, contentSize))
            {
                media_url_handle_ptr urlHandle = media_url_handle_ptr( new media_url_handle(videoUrl, mediaType, contentSize, cookies, "") );

                const media_content_type_t mediaType_ = ::openmedia::downloader::parse_media_content_type(mediaType);

                media_quality_type_t media_quality_type = ((i == 0) ? mediaQuality360P : mediaQuality480P);
                media_video_type_t media_video_codec_type = mediaVideoH264;
                media_audio_type_t media_audio_codec_type = mediaAudioAAC;

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

    }

    ::std::sort(mediaInfoList.begin(), mediaInfoList.end(), &compare_media_download_info);
    
    boost::shared_ptr<media_download_list::Impl> media_download_list_impl( new media_download_list::Impl(titleW, titleUtf8, thumbnail, duration, Url) );
    media_download_list_impl->get_array().swap(mediaInfoList);

    return media_download_list_ptr( new media_download_list(media_download_list_impl) );
}

} } } 
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

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace openmedia { namespace downloader { namespace vimeo {


std::string parse_clip_id(const std::string & content)
{
    boost::regex re( std::string("value\\s*=\\s*\"([\\d]+)\"\\s+id=\\s*\"clip_id\"") );
    boost::sregex_token_iterator i(content.begin(), content.end(), re, 1);
    boost::sregex_token_iterator j;
    if (i != j)
    {
        std::string id = (*i).str();
        return id;
    } else
    {
        re.set_expression(std::string("id=\\s*\"clip_id\"\\s+value\\s*=\\s*\"([\\d]+)\""));
        boost::sregex_token_iterator i(content.begin(), content.end(), re, 1);
        boost::sregex_token_iterator j;
        if (i != j)
        {
            std::string id = (*i).str();
            return id;
        } else
            return "";
    }
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

    std::vector<HttpHeader> parsedHeaders;
    parse_http_headers(main_headers, parsedHeaders); 
    const std::string cookies = use_cookies(parsedHeaders);

    const std::string clipId = (clipIdUrl == "") ? (parse_clip_id(main_content)) : (clipIdUrl);
    const std::string quertyUrl = std::string("http://www.vimeo.com/moogaloop/load/clip:") + clipId;

    std::string query_headers, query_content;
    if (!download_page(quertyUrl, query_headers, query_content, cookies, "Keep-Alive"))
        throw url_parser::exception();

    std::wstringstream strm;
    strm << utf8_to_utf16(query_content);


    boost::property_tree::basic_ptree<std::wstring, std::wstring> pt;
    boost::property_tree::xml_parser::read_xml(strm, pt);

    const std::string thumbUrl = utf16_to_utf8(pt.get_optional<std::wstring>(L"xml.video.thumbnail").get_value_or(L""));

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

    const std::string durationStr = utf16_to_utf8(pt.get_optional<std::wstring>(L"xml.video.duration").get_value_or(L"0"));

    int duration = 0;
    try
    {
        duration = boost::lexical_cast<int>( durationStr ) * 1000;
    } 
    catch(...)
    {
    }

    const std::string titleUtf8 = utf16_to_utf8(pt.get_optional<std::wstring>(L"xml.video.caption").get_value_or(L"Unknown"));
    const std::wstring titleW = ::openmedia::utf8_to_utf16(titleUtf8);
    const std::string widthStr = utf16_to_utf8(pt.get_optional<std::wstring>(L"xml.video.width").get_value_or(L"0"));
    const std::string heightStr = utf16_to_utf8(pt.get_optional<std::wstring>(L"xml.video.height").get_value_or(L"0"));
    
    int width = 0, height = 0;
    try {
        width = boost::lexical_cast<int>(widthStr);
        height = boost::lexical_cast<int>(heightStr);
    }catch(...)
    {}
    
    bool isHd = ("0" != utf16_to_utf8(pt.get_optional<std::wstring>(L"xml.video.isHD").get_value_or(L"0")));

    std::string token1 = utf16_to_utf8(pt.get_optional<std::wstring>(L"xml.request_signature").get_value_or(L""));
    std::string token2 = utf16_to_utf8(pt.get_optional<std::wstring>(L"xml.request_signature_expires").get_value_or(L""));

    if (token1 == "" || token2 == "")
         throw url_parser::exception();

    std::vector< media_download_info > mediaInfoList;

    const std::string queryBase = std::string("http://www.vimeo.com/moogaloop/play/clip:") + clipId + "/" 
            + token1 + "/" + token2;

    for (int i = 0; i < (isHd ? 2: 1); ++i)
    {
        std::string query = ((i == 0) ? (queryBase + "/?q=sd&type=local&embed_location=") : (queryBase + "/?q=hd&type=local&embed_location="));
        
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
             const media_quality_type_t media_quality_type = ((i == 0) ? mediaQuality360P : mediaQuality720P);
             const media_video_type_t media_video_codec_type = mediaVideoH264;
             const media_audio_type_t media_audio_codec_type = mediaAudioAAC;

             int width_ = 0, height_ = 0;
             if (isHd && i == 1)
             {
                 width_ = width; 
                 height_ = height;
             }

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

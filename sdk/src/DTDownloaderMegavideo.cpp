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

#include "DTDownloaderMegavideo.h"
#include "DTDownloaderMegavideoScript.h"
#include "DTDownloaderUtils.h"
#include "DTMediaDownloaderDetails.h"
#include "DTSimpleMediaParser.h"
#include <openmedia/DTMediaDownloader.h>
#include <openmedia/DTString.h>
#include <openmedia/DTHttpDownloader.h>
#include <set>

#include "DTMediaDownloaderDetails.h"

#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/regex.hpp>
#include <boost/bind.hpp>
#define BOOST_THREAD_USE_LIB
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>

namespace openmedia { namespace downloader { namespace megavideo {

std::string get_palyer_url(const std::string & Content)
{
    boost::regex re( std::string("embedSWF\\s*\\(\\s*\"([^\"]+)\"") );
    boost::sregex_token_iterator i(Content.begin(), Content.end(), re, 1);
    boost::sregex_token_iterator j;
    if (i != j)
    {
        const std::string seq = (*i).str();
        return seq;    
    }
    else
        return "";
}

std::string extract_prop(const std::string & Seq, const std::string & Prop)
{
    boost::regex re( std::string("flashvars\\s*\\.\\s*") + Prop + "\\s*=\\s*\"([^\"]+)\"");
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

std::string extract_prop2(const std::string & Seq, const std::string & Prop)
{
    boost::regex re( Prop + std::string("\\s*=\\s*\"([^\"]+)\"") );
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

namespace {
class Downloader
{
public:
    Downloader(const std::string & Url, const std::string & Cookies, std::ostream & OutputStream) :
      onExit_(false),
      error_(false),
      http_downloader_(
          new openmedia::downloader::http_downloader( Url, Cookies, OutputStream, 
          boost::bind(&Downloader::StateNotify, this, _1), 
          boost::bind(&Downloader::ProgressNotify, this, _1) )
          )
      {
          while (!onExit_)
          {
              boost::this_thread::sleep( boost::posix_time::milliseconds(100) ); 
          }
      }

      void cancel()
      {
          http_downloader_->cancel();      
      }

      void pause()
      {
          http_downloader_->pause();        
      }

      void resume()
      {
          http_downloader_->resume();        
      }

      bool error()
      {
          return error_;      
      }

private:
    bool onExit_;
    bool error_;
    boost::scoped_ptr< openmedia::downloader::http_downloader > http_downloader_;
    
    void ProgressNotify(boost::uint64_t/* Bytes*/)
    {
    }

    void StateNotify(http_downloader::download_state_t State)
    {
        switch(State)
        {
        case http_downloader::stateCancel:
        case http_downloader::stateNull:
        case http_downloader::stateError:
        default:
            error_= true;
        case http_downloader::stateFinish:
            onExit_ = true;
            break;
        case http_downloader::stateResume:
        case http_downloader::stateDownload:
        case http_downloader::stateStart:
        case http_downloader::statePause:
            break;
        }
    }
};

}

media_download_list_ptr parse_url(const std::string & Url)
{
    std::string main_headers, main_content;
    if (!download_page(Url, main_headers, main_content, "", ""))
        throw url_parser::exception();

    std::vector<HttpHeader> parsedHeaders;
    parse_http_headers(main_headers, parsedHeaders); 
    const std::string cookies = use_cookies(parsedHeaders);

    //const std::string playerUrl = get_palyer_url(main_content);

    const std::string v = extract_prop(main_content, "v");

    const std::string infoUrl = std::string("http://www.megavideo.com/xml/videolink.php?v=") + v;

    int duration = 0;
    std::string info_headers, info_content;
    if (download_page(infoUrl, info_headers, info_content, cookies, ""))
    {
        const std::string durationStr = extract_prop2(info_content, "runtime");
        try 
        {
            duration = 1000 * boost::lexical_cast<long>(durationStr);
        }
        catch(...)
        {}
    }

    const std::string un = extract_prop(main_content, "un");
    const std::string key1 = extract_prop(main_content, "k1");
    const std::string key2 = extract_prop(main_content, "k2");
    const std::string s = extract_prop(main_content, "s");

    const std::string hd_un = extract_prop(main_content, "hd_un");
    const std::string hd_key1 = extract_prop(main_content, "hd_k1");
    const std::string hd_key2 = extract_prop(main_content, "hd_k2");
    const std::string hd_s = extract_prop(main_content, "hd_s");

    const std::string thumbUrl = extract_prop(main_content, "image");

    media_download_list::BytesArrayPtr thumbnail = 
        media_download_list::BytesArrayPtr( new media_download_list::BytesArray( 0 ) );

#if 0
    try 
    {
        ::std::stringstream strcontent__;
        Downloader d(thumbUrl, "", strcontent__);
        if (!d.error())
        {
            std::string content = strcontent__.str();
            thumbnail = media_download_list::BytesArrayPtr( new media_download_list::BytesArray( content.size() ) );
            ::std::copy( content.begin(), content.end(), thumbnail->begin() );
        }
    }
    catch(...)
    {
    }
#endif
    
#if 0

    try 
    {
        std::string headers__, content__;
        if ((thumbUrl != "") && download_page(thumbUrl, headers__, content__, "", "keep-alive"))
        {
            thumbnail = media_download_list::BytesArrayPtr( new media_download_list::BytesArray( content__.size() ) );
            ::std::copy( content__.begin(), content__.end(), thumbnail->begin() );
        };
    }
    catch(...)
    {

    }
#endif

    std::string title = extract_prop(main_content, "title");
    boost::algorithm::replace_all(title, "+", " ");
    std::wstring titleW = ::openmedia::utf8_to_utf16(title);

    std::string url = "", hd_url = "";
    if (hd_un.length() > 0)
    {
        hd_url = std::string("http://www") + hd_s + ".megavideo.com/files/" + DecryptTokenUrl(hd_un, hd_key1, hd_key2) + "/";     
    }

    if (un.length() > 0)
    {
        url = std::string("http://www") + s + ".megavideo.com/files/" + DecryptTokenUrl(un, key1, key2) + "/";     
    }

    std::vector< media_download_info > mediaInfoList;

    for (int i = 0; i < 2; ++i)
    {
        std::string videoUrl = (i==0) ? url : hd_url;
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

            media_quality_type_t media_quality_type = ((i == 0) ? mediaQuality360P : mediaQuality720P);
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

    ::std::sort(mediaInfoList.begin(), mediaInfoList.end(), &compare_media_download_info);

    boost::shared_ptr<media_download_list::Impl> media_download_list_impl( new media_download_list::Impl(titleW, title, thumbnail, duration, Url) );
    media_download_list_impl->get_array().swap(mediaInfoList);

    return media_download_list_ptr( new media_download_list(media_download_list_impl) );
}


} } }

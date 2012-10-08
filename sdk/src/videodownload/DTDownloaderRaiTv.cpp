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

#include "DTDownloaderRaiTv.h"
#include "../DTDownloaderUtils.h"
#include "../DTMediaDownloaderDetails.h"
#include "../DTSimpleMediaParser.h"
#include <openmedia/DTMediaDownloader.h>
#include <openmedia/DTString.h>
#include <set>

#include "../DTMediaDownloaderDetails.h"

#include <string>
#include <vector>
#include <iostream>

#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

namespace openmedia { namespace downloader { namespace raitv {

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
}

#define DT_SEARCH_CONTENT(A) searchContent(Content.begin(), Content.end(), (A) )

std::string get_thumb_url(const std::string & Content)
{
    // <meta property="og:image" content="http://www.rai.tv/dl/img/2012/09/105x791348636392316italys-debt_280xFree.jpg"/>
    const std::string url = DT_SEARCH_CONTENT("<meta.*?property=\"og:image\".*?content=\"([^\"]*?)\"");
    return url;
}

std::string get_title(const std::string & Content)
{
    // <meta name="title" content="Monti a New York: &quot;Non mi candido&quot;"/>
    std::string title = DT_SEARCH_CONTENT("<meta.*?name=\"title\".*?content=\"([^\"]*?)\"");
    replace_xml_esc(title);
    return title;
}

std::string get_video_url(const std::string & Content)
{
    // <meta name="videourl_mp4" content="http://mediapolisvod.rai.it/relinker/relinkerServlet.htm?cont=OiG2ossSlash52cLoeeqqEEqual"/>
    std::string url = DT_SEARCH_CONTENT("<meta.*?name=\"videourl_mp4\".*?content=\"([^\"]*?)\"");
    if (url.empty())
    {
        //var videoURL = "http://mediapolisvod.rai.it/relinker/relinkerServlet.htm?cont=OiG2ossSlash52cLoeeqqEEqual";
        url = DT_SEARCH_CONTENT("var\\s*videoURL\\s*=\\s*\"([^\"]*?)\"");
    }

    return url;
}

media_download_list_ptr parse_url(const std::string & Url)
{
    std::string main_headers, main_content, info_headers, info_content;
    if (!download_page(Url, main_headers, main_content, "", ""))
        throw url_parser::exception();

    std::vector<HttpHeader> parsedHeaders;
    parse_http_headers(main_headers, parsedHeaders); 

    media_download_list::BytesArrayPtr thumbnail;

    const std::string cookies = use_cookies(parsedHeaders);


    std::string thumbUrl = get_thumb_url(main_content);
    std::string title = get_title(main_content);
    std::wstring titleW = ::openmedia::utf8_to_utf16(title);

    int duration = 0;

    try 
    {
        std::string headers__, content__;
        if (download_page(thumbUrl, headers__, content__, "", ""))
        {
            thumbnail = media_download_list::BytesArrayPtr( new media_download_list::BytesArray( content__.size() ) );
            ::std::copy( content__.begin(), content__.end(), thumbnail->begin() );
        };
    }
    catch(...) {}

    boost::shared_ptr<media_download_list::Impl> media_download_list_impl(
        new media_download_list::Impl(titleW, title, thumbnail, duration, Url) );

    const std::string video_url = get_video_url(main_content);

    std::pair<bool, media_download_info> res;
    res = parse_video(video_url, cookies, mediaQuality360P, mediaVideoH264, mediaAudioAAC);
    if (res.first)
        media_download_list_impl->get_array().push_back(res.second);

    return media_download_list_ptr( new media_download_list(media_download_list_impl) );
}

}
}
}

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

#include <openmedia/DTMediaSite.h>
#include <urdl/url.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include "DTDownloaderYoutubeDetails.h"
#include <boost/regex.hpp>

namespace openmedia { namespace downloader {

media_site_type_t media_site_utils::validate_url(const std::string & Url)
{
    std::string url_to_parse = Url;
    if (!( boost::algorithm::contains(url_to_parse, "http://") ||
        boost::algorithm::contains(url_to_parse, "https://") ))
        url_to_parse = "http://" + url_to_parse;

    boost::regex matchUrl("(https?|ftp|file)://[-A-Za-z0-9+&@#/%?=~_|!:,.;]*[-A-Za-z0-9+&@#/%=~_|]");

    boost::match_results< std::string::const_iterator > res;
    if (boost::regex_search(url_to_parse, res, matchUrl, boost::match_default))
    {
        url_to_parse = res[0];
    }
    else
        return mediaSiteNull;
    
    urdl::url url(url_to_parse);
    if (boost::algorithm::contains(url.host(), std::string("youtube")) || 
        boost::algorithm::contains(url.host(), std::string("youtu.be")))
    {
        return mediaSiteYoutube;    
    }
    else if (boost::algorithm::contains(url.host(), std::string("rai.tv")))
    {
        return mediaSiteRaiTv;    
    }
    else if (boost::algorithm::contains(url.host(), std::string("vimeo")))
    {
        return mediaSiteVimeo;    
    } 
    else if (boost::algorithm::contains(url.host(), std::string("dailymotion")))
    {
        return mediaSiteDailymotion;
    }
    else if (boost::algorithm::contains(url.host(), std::string("facebook")))
    {
        return mediaSiteFacebook;
    }
    else if (boost::algorithm::contains(url.host(), std::string("metacafe")))
    {
        return mediaSiteMetacafe;
    }
    else
    {
        return mediaSiteUnknown;
    }
}

 bool media_site_utils::is_contain_playlist(const std::string & Url)
 {
     if (mediaSiteYoutube == validate_url(Url) && youtube::is_contain_playlist_impl(Url))
         return true;
     else
         return false;
 }

} }

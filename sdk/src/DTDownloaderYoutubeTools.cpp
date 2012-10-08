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

#include  "DTDownloaderYoutubeTools.h"
#include  "DTDownloaderYoutubeDetails.h"
#include  "DTDownloaderUtils.h"

#include  <openmedia/DTString.h>

#include <urdl/istream.hpp>
#include <urdl/http.hpp>
#include <sstream>
#include <boost/foreach.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <boost/lexical_cast.hpp>


namespace openmedia { namespace downloader { namespace youtube {

bool receive_youtube_info_impl(const std::string & Url,
                          const std::string & ConnectionType,
                          const std::string & ContentType,
                          const std::string & UserAgent,
                          const std::string & Content,
                          std::string & Result
                          )
{
    urdl::istream is;
    is.set_option(urdl::http::request_method("POST"));
    is.set_option(urdl::http::connection_type(ConnectionType));
    is.set_option(urdl::http::request_content_type(ContentType));
    is.set_option(urdl::http::user_agent(UserAgent));
    is.set_option(urdl::http::request_content(Content));
    is.open(Url);

    if (!is)
    {
        std::cerr << "[openmedia::downloader::receive_youtube_info] unable to open URL: ";
        std::cerr << is.error().message() << std::endl;
        return false;
    }

    std::stringstream outStrm;
    outStrm << is.rdbuf();
    Result = outStrm.str();
    return true;
}

bool receive_youtube_info_impl2(const std::string & UserAgent, const ::std::vector< std::string > & Ids, std::string & Result)
{
    std::string Content = std::string("video_ids=");
    bool f = true;
    BOOST_FOREACH(const std::string & id, Ids)
    {
        if (!f)
            Content += "%2C";

        if (f)
            f = false;

        Content += id;
    }

    const std::string Url = 
        "http://www.youtube.com/video_info_ajax?action_get_videos_data=1&count=" + boost::lexical_cast<unsigned int>(Result.size());

    return receive_youtube_info_impl(Url, "", "application/x-www-form-urlencoded", UserAgent, Content, Result);
}

bool receive_youtube_info(const std::string & UserAgent, const ::std::vector< std::string > & Ids, std::vector<youtube_desc> & Result)
{
    std::vector<youtube_desc> res;
    std::string result;
    if (!receive_youtube_info_impl2(UserAgent, Ids, result))
        return false;

    result = replace_u_codes(result);

    try 
    {
        std::stringstream strm;
        strm << result;

        typedef boost::property_tree::ptree Tree_;
        Tree_ pt;
        boost::property_tree::json_parser::read_json(strm, pt);

        BOOST_FOREACH(const Tree_::value_type & t, pt)
        {
            const std::string Key = t.first;

            if (Key == "data")
            {
                const Tree_ & Data = t.second;
                
                BOOST_FOREACH(const Tree_::value_type & t, Data)
                {

                    const std::string Key = t.first;
                    const Tree_ & Target = t.second;

                    const std::string username = Target.get_optional<std::string>("username").get_value_or("");
                    const std::string encrypted_id = Target.get_optional<std::string>("encrypted_id").get_value_or("");
                    const std::string title = Target.get_optional<std::string>("title").get_value_or("");
                    const std::wstring titlew = utf8_to_utf16(title);

                    res.push_back( youtube_desc(username, encrypted_id, title, titlew) );

                }
            }
        }
    }
    catch(...)
    {
        return false;    
    }

    Result = res;
    return true;
}


} } }


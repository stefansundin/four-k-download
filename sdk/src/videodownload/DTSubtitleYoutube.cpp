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


#include "DTHeadersVideoDownload.h"
#include "DTSubtitleYoutube.h"

#include <sstream>
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <openmedia/DTCommon.h>
#include <openmedia/DTError.h>
#include "../DTDownloaderUtils.h"
#include "DTSubtitleImpl.h"


namespace openmedia {
namespace downloader {
namespace youtube {

namespace {

std::string download_subtitle_xml_by_id(const std::string & youTubeId)
{
    const std::string url = "http://video.google.com/timedtext?type=list&v=" + youTubeId;
    std::string headers, content;
    if (!download_page(url, headers, content, "", ""))
    {
        BOOST_THROW_EXCEPTION(::openmedia::errors::error() << boost::errinfo_api_function("download_page"));
        DT_IF_DISABLE_EXCEPTIONS(return "");
    }
    return content;
}
}

std::vector<subtitle_track> get_subtitle_list_by_id(const std::string & youTubeId)
{
    typedef boost::property_tree::ptree Tree;
    Tree pt;

    std::stringstream input_xml;
    input_xml << download_subtitle_xml_by_id(youTubeId);
    boost::property_tree::xml_parser::read_xml(input_xml, pt);

    std::vector<subtitle_track> result;

    BOOST_FOREACH(const Tree::value_type & t, pt)
    {
        const std::string key = t.first;
        const Tree & transcript_list = t.second;

        BOOST_FOREACH(const Tree::value_type & t, transcript_list)
        {
            const std::string key = t.first;
            if (key != "track")
                continue;

            const Tree & track = t.second;
            BOOST_FOREACH(const Tree::value_type & t, track)
            {
                const std::string key = t.first;
                if (key == "<xmlattr>")
                {
                    const std::string id = t.second.get_optional<std::string>("id").get_value_or("");
                    const std::string name = t.second.get_optional<std::string>("name").get_value_or("");
                    const std::string lang_code = t.second.get_optional<std::string>("lang_code").get_value_or("");
                    const std::string lang_original = t.second.get_optional<std::string>("lang_original").get_value_or("");
                    const std::string lang_translated = t.second.get_optional<std::string>("lang_translated").get_value_or("");
                    const std::string lang_default_str = t.second.get_optional<std::string>("lang_default").get_value_or("false");                   
                    bool lang_default = false;
                    if (lang_default_str == "1" || lang_default_str == "true")
                        lang_default = true;
                    const std::string url = "http://video.google.com/timedtext?type=track&name=" + url_encode(name) + 
                        "&lang=" + lang_code + "&v=" + youTubeId;
                    subtitle_track track_desc = 
                    {
                        id,
                        name,
                        lang_code,
                        lang_original,
                        lang_translated,
                        lang_default,
                        url
                    };
                    result.push_back(track_desc);
                }
            }
        }
    }

    return result;    
}

namespace {
std::string download_subtitles(const std::string & url)
{
    std::string headers, content;
    if (!download_page(url, headers, content, "", ""))
    {
        BOOST_THROW_EXCEPTION(::openmedia::errors::error() << boost::errinfo_api_function("download_page"));
        DT_IF_DISABLE_EXCEPTIONS(return "");
    }
    return content;
}
}

void get_subtitles(const std::string & url, std::vector<transcript_element> & result)
{
    typedef boost::property_tree::ptree Tree;
    Tree pt;

    std::stringstream input_xml;
    input_xml << download_subtitles(url);
    boost::property_tree::xml_parser::read_xml(input_xml, pt);

    BOOST_FOREACH(const Tree::value_type & t, pt)
    {
        const std::string key = t.first;
        const Tree & transcript = t.second;

        BOOST_FOREACH(const Tree::value_type & t, transcript)
        {
            const std::string key = t.first;
            if (key != "text")
                continue;
            
            try 
            {
                const Tree & text = t.second;
                
                const std::string data = text.data();               

                BOOST_FOREACH(const Tree::value_type & t, text)
                {
                    const std::string key = t.first;
                    if (key == "<xmlattr>")
                    {
                        const std::string start_str = t.second.get_optional<std::string>("start").get_value_or("");
                        const std::string duration_str = t.second.get_optional<std::string>("dur").get_value_or("");
                        const double start = boost::lexical_cast<double>(start_str);
                        const double duration = boost::lexical_cast<double>(duration_str);

                        transcript_element elm = {start, duration, data};
                        result.push_back(elm);
                        break;
                    }
                }
            }
            catch(...)
            {
            }
        }
    }
}


}


class subtitles_youtube_impl : public subtitles::Impl
{
public:
    subtitles_youtube_impl(const std::string & youTubeId, const std::string & transcribed_url) 
    {
        try 
        {
            tracks_ = youtube::get_subtitle_list_by_id(youTubeId);

            bool transcribedEnabled = true;
            try 
            {
                std::vector<transcript_element> transcribed;
                youtube::get_subtitles(transcribed_url, transcribed);
                if (transcribed.empty())
                    transcribedEnabled = false;

            }
            catch(...)
            {
                transcribedEnabled = false;
            }

            if (transcribedEnabled)
            {
                subtitle_track transcribed_track = {"", "transcribed", "en", "English (transcribed)", "English (transcribed)", false, transcribed_url};
                tracks_.push_back(transcribed_track);
            }

        }
        catch(...)
        {
        }
    }

public:
    virtual std::vector<subtitle_track> & tracks()
    {
        return tracks_;
    }

    virtual void download_subtitle(size_t index, std::vector<transcript_element> & result)
    {
        result.clear();
        try
        {
            youtube::get_subtitles(tracks_.at(index).url, result);                
        }
        catch(...)
        {
        }
    }

private:
    std::vector<subtitle_track> tracks_;

};


subtitles_youtube::subtitles_youtube(const std::string & youtubeId, const std::string & transcribed_url): 
subtitles( new subtitles_youtube_impl(youtubeId, transcribed_url) )
{
        
}


}
}

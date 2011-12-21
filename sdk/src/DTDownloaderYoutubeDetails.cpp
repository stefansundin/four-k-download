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

#include "DTDownloaderYoutubeDetails.h"
#include "DTDownloaderUtils.h"
#include <openmedia/DTMediaDownloader.h>

#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

namespace openmedia { namespace downloader { namespace youtube {

/// Example
/// Content: " ... swfConfig = { ... "t" : "vjVQa1PpcFOq2HnvO4pUZS8orRurXg_BPUfTNaxdtiE=" ... }
/// propname: "t"
/// Output: "vjVQa1PpcFOq2HnvO4pUZS8orRurXg_BPUfTNaxdtiE"
bool get_youtube_property(const std::string & content, const char * propname, std::string & propValue)
{
    if (get_youtube_property_impl1(content, propname, propValue))
        return true;
    else
        return get_youtube_property_impl2(content, propname, propValue);
}

bool get_youtube_property_impl1(const std::string & content, const char * propname, std::string & propValue)
{
    //boost::regex re( std::string("swfConfig\\s*=\\s*[\\{][^\\}]*\"") +  propname + "\"\\s*:\\s*\"([^\"]*)\"");
    //boost::regex re( std::string("swfConfig\\s*=\\s*[\\{].*\"") +  propname + "\"\\s*:\\s*\"([^\"]*)\"");
    boost::regex re( std::string("'PLAYER_CONFIG'\\s*:\\s*[\\{].*\"") +  propname + "\"\\s*:\\s*\"([^\"]*)\"");
    boost::sregex_token_iterator i(content.begin(), content.end(), re, 1);
    boost::sregex_token_iterator j;
    if (i != j)
    {
        propValue = (*i).str();
        return true;
    } else
        return false;
}

bool get_youtube_property_impl2(const std::string & content, const char * propname, std::string & propValue)
{
    boost::regex re( std::string("\"") +  propname + "\"\\s*:\\s*\"([^\"]*)\"");
    boost::sregex_token_iterator i(content.begin(), content.end(), re, 1);
    boost::sregex_token_iterator j;
    if (i != j)
    {
        propValue = (*i).str();
        return true;
    } else
        return false;
}

bool get_youtube_property_d(const std::string & content, const char * propname, std::string & propValue)
{
    if (get_youtube_property_d_impl1(content, propname, propValue))
        return true;
    else
        return get_youtube_property_d_impl2(content, propname, propValue);
}

bool get_youtube_property_d_impl1(const std::string & content, const char * propname, std::string & propValue)
{
    //boost::regex re( std::string("swfConfig\\s*=\\s*[\\{][^\\}]*\"") +  propname + "\"\\s*:\\s*\"([^\"]*)\"");
    boost::regex re( std::string("'PLAYER_CONFIG'\\s*:\\s*[\\{].*\"") +  propname + "\"\\s*:\\s*([\\d]*)");
    boost::sregex_token_iterator i(content.begin(), content.end(), re, 1);
    boost::sregex_token_iterator j;
    if (i != j)
    {
        propValue = (*i).str();
        return true;
    } else
        return false;
}

bool get_youtube_property_d_impl2(const std::string & content, const char * propname, std::string & propValue)
{
    boost::regex re( std::string("\"") +  propname + "\"\\s*:\\s*([\\d]*)");
    boost::sregex_token_iterator i(content.begin(), content.end(), re, 1);
    boost::sregex_token_iterator j;
    if (i != j)
    {
        propValue = (*i).str();
        return true;
    } else
        return false;
}

bool search_watch_url2(const std::string & content, std::string & value)
{
    ::std::string video_id;
    if (get_youtube_property_impl2(content, "video_id", video_id))
    {
        value = video_id;
        return true;
    }
    else
        return false;
}

bool search_watch_url(const std::string & content, std::string & value)
{
    boost::regex re( "watch\\?v=([^\"&\\s\\\\]+)" );
    boost::sregex_token_iterator i(content.begin(), content.end(), re, 1);
    boost::sregex_token_iterator j;
    if (i != j)
    {
        value = (*i).str();
        return true;
    } 
    else if (search_watch_url2(content, value))
    {
        return true;
    }
    else 
        return false;
}


std::string normalize_youtube_url(const std::string & Url)
{
    std::string url = Url;
    if ( boost::algorithm::contains(Url, std::string("/user/") ) )
    {
        //boost::regex re( "/user/.+/([^/]+)" );
        boost::regex re( "/user/.+/([-_a-zA-Z0-9]+)" );
        boost::sregex_token_iterator i(url.begin(), url.end(), re, 1);
        boost::sregex_token_iterator j;
        if (i != j)
        {
            url = "http://www.youtube.com/watch?v=" + (*i).str();
        } 
        else
        {
            boost::regex re("/user/.+v=([-_a-zA-Z0-9]+)");
            boost::sregex_token_iterator i(url.begin(), url.end(), re, 1);
            boost::sregex_token_iterator j;
            if (i != j)
            {
                url = "http://www.youtube.com/watch?v=" + (*i).str();
            }
        }
    }

    return url;
}

std::string remove_last_comma(const std::string & Url)
{
    std::vector<std::string> resultVal;
    boost::regex re("([^,]*)[,][\\d]+\\z");
    //int index[] = {1,2};
    boost::sregex_token_iterator i(Url.begin(), Url.end(), re, 1);
    boost::sregex_token_iterator j;
    while(i != j)
    {
        return (*i++).str();
    }
    return Url;
}

struct replace_fmt 
{
    std::string operator () (boost::smatch const & what)
    {
        return std::string("") + (char)(boost::lexical_cast<int>(what[1].str()));
    }
};

std::string replace_html_codes(const std::string & input)
{
    return boost::regex_replace(input, boost::regex( "&#([\\d]+);" ), replace_fmt(), boost::match_default | boost::format_all );
}

bool parse_title_from_metatag(const std::string & content, std::string & Title)
{
    boost::regex re("<meta[\\s]+name[\\s]*=[\\s]*\"title\"[\\s]+content[\\s]*=[\\s]*\"([^\"]*)\"");
    boost::sregex_token_iterator i(content.begin(), content.end(), re, 1);
    boost::sregex_token_iterator j;
    if (i != j)
    {
        Title = (*i++).str();
        return true;
    }
    else 
        return false;    
}

//    <meta name="title" content="Mortal Kombat: New Fatalities Highlights">
void extract_youtube_title(const std::string & content, std::string & Title)
{
    std::string tmpTitle;
    if (! (parse_title_from_metatag(content, tmpTitle) || get_youtube_property(content, "title", tmpTitle)) )
        return;

    Title = replace_html_codes(tmpTitle);
    replace_xml_esc(Title);
    return;
}

bool parse_thumb_url_from_metatag(const std::string & content, std::string & ThumbUrl)
{
    boost::regex re("<meta[\\s]+property[\\s]*=[\\s]*\"og:image\"[\\s]+content[\\s]*=[\\s]*\"([^\"]*)\"");
    boost::sregex_token_iterator i(content.begin(), content.end(), re, 1);
    boost::sregex_token_iterator j;
    if (i != j)
    {
        ThumbUrl = (*i++).str();
        return true;
    }
    else
        return false;
}

bool parse_thumb_url_from_prop(const std::string & content, std::string & ThumbUrl)
{
    std::string tmpUrl;
    if (get_youtube_property(content, "iurl", tmpUrl))
    {
        boost::algorithm::replace_all(tmpUrl, "\\/", "/");
        ThumbUrl = tmpUrl;
        return true;
    }
    else
        return false;
}


// <meta property="og:image" content="http://i4.ytimg.com/vi/KD84r_GIBrw/default.jpg"> 
std::string extract_youtube_thumbnail_url(const std::string & content)
{
    std::string tmpUrl = "";
    if (!(parse_thumb_url_from_metatag(content, tmpUrl) || parse_thumb_url_from_prop(content, tmpUrl)) )
        return "";

    return tmpUrl;
}

void split_youtube_video_info_impl(const std::string & videoInfo, std::vector<std::string> & urls, std::vector<int> &Ids)
{
    std::vector<std::string> resultVal;
    std::vector<int> idsVal;

    //boost::regex re("[\\|](http://[^/]+/videoplayback[^,^\\|]*)[,\\|]");
    boost::regex re("([\\d]+)[\\|](?=(http://[^/]+/videoplayback[^\\|]*))");
    int index[] = {1,2};
    boost::sregex_token_iterator i(videoInfo.begin(), videoInfo.end(), re, index);
    boost::sregex_token_iterator j;
    while(i != j)
    {
        idsVal.push_back( boost::lexical_cast<int>((*i++).str()) );        
        resultVal.push_back( remove_last_comma((*i++).str()) );
    }

    Ids.swap(idsVal);
    urls.swap(resultVal);    
}

void split_youtube_video_info_old(const std::string & videoInfo, std::vector<std::string> & urls, std::vector<int> &Ids)
{
    boost::regex re("fmt_url_map=([^&]+)[&]");
    int index[] = {1};
    boost::sregex_token_iterator i(videoInfo.begin(), videoInfo.end(), re, index);
    boost::sregex_token_iterator j;
    std::string fmt_url_map = "";
    while(i != j)
    {
        fmt_url_map = url_decode((*i).str());                
        break;
    }

    if (fmt_url_map.size())
        return split_youtube_video_info_impl(fmt_url_map, urls, Ids);
}

void split_youtube_video_info(const std::string & videoInfo, std::vector<std::string> & urls, std::vector<int> &Ids)
{
    split_youtube_video_info2(videoInfo, urls, Ids);
    if (!Ids.size())
    {
        split_youtube_video_info_old(videoInfo, urls, Ids);
    }
}

void split_youtube_video_info_impl2(const std::string & videoInfo, std::vector<std::string> & urls, std::vector<int> &Ids)
{
    std::vector<std::string> resultVal;
    std::vector<int> idsVal;

    boost::regex re("url=([^&]+)&.+?&itag=([\\d]+)");
    int index[] = {1,2};
    boost::sregex_token_iterator i(videoInfo.begin(), videoInfo.end(), re, index);
    boost::sregex_token_iterator j;
    while(i != j)
    {
        resultVal.push_back( url_decode((*i++).str()) );
        idsVal.push_back( boost::lexical_cast<int>((*i++).str()) );        
    }

    Ids.swap(idsVal);
    urls.swap(resultVal);    
}

void split_youtube_video_info_impl3(const std::string & videoInfo, std::vector<std::string> & urls, std::vector<int> &Ids)
{
    std::vector<std::string> resultVal;
    std::vector<int> idsVal;

    ::std::vector< ::std::string > splitUrls;
    boost::algorithm::split(splitUrls, videoInfo, boost::algorithm::is_any_of(","));
    if (splitUrls.size())
    {
        BOOST_FOREACH( ::std::string & url, splitUrls )
        {
            boost::regex re("url=([^&]+)&.+?itag=([\\d]+)");
            int index[] = {1,2};
            boost::sregex_token_iterator i(url.begin(), url.end(), re, index);
            boost::sregex_token_iterator j;
            if (i != j)
            {
                resultVal.push_back( url_decode((*i++).str()) );
                idsVal.push_back( boost::lexical_cast<int>((*i++).str()) );        
            }
        }
    }

    Ids.swap(idsVal);
    urls.swap(resultVal);    
}

void split_youtube_video_info2(const std::string & videoInfo, std::vector<std::string> & urls, std::vector<int> &Ids)
{
    boost::regex re("url_encoded_fmt_stream_map=([^&]+)[&]");
    int index[] = {1};
    boost::sregex_token_iterator i(videoInfo.begin(), videoInfo.end(), re, index);
    boost::sregex_token_iterator j;
    std::string fmt_url_map = "";
    while(i != j)
    {
        fmt_url_map = url_decode((*i).str());                
        break;
    }

    if (fmt_url_map.size())
        return split_youtube_video_info_impl3(fmt_url_map, urls, Ids);
}

std::string get_youtube_token(const std::string & content)
{
    std::string token = "";
    get_youtube_property(content, "t", token);
    return token;
}

boost::int64_t get_youtube_duration(const std::string & content)
{
    std::string length = "";
    get_youtube_property_d(content, "length_seconds", length);
    int d = 0;
    try 
    {
        d = boost::lexical_cast< boost::int64_t >( length );
    } 
    catch(...)
    {
    }
    return d;
}

std::string get_youtube_video_id(const std::string & content)
{
    std::string video_id = "";
    get_youtube_property(content, "video_id", video_id);
    return video_id;
}

void youtube_format_map(int Id, media_quality_type_t & media_quality_type, 
                        media_content_type_t & media_content_type,
                        media_video_type_t & media_video_codec_type,
                        media_audio_type_t & media_audio_codec_type)
{
    media_quality_type = mediaQualityUnknown;
    media_content_type = mediaContentUnknown;
    media_video_codec_type = mediaVideoUnknown;
    media_audio_codec_type = mediaAudioUnknown;
    switch(Id)
    {
    case 43:
        media_quality_type = mediaQuality360P;
        media_content_type = mediaContentVideoWebm;
        media_video_codec_type = mediaVideoVP8;
        media_audio_codec_type = mediaAudioVorbis;
        break;
    case 44:
        media_quality_type = mediaQuality480P;
        media_content_type = mediaContentVideoWebm;
        media_video_codec_type = mediaVideoVP8;
        media_audio_codec_type = mediaAudioVorbis;
        break;
    case 45:
        media_quality_type = mediaQuality720P;
        media_content_type = mediaContentVideoWebm;
        media_video_codec_type = mediaVideoVP8;
        media_audio_codec_type = mediaAudioVorbis;
        break;
    case 38:
        media_quality_type = mediaQualityOriginal;
        media_content_type = mediaContentVideoMP4;
        media_video_codec_type = mediaVideoH264;
        media_audio_codec_type = mediaAudioAAC;
        break;
    case 37:
        media_quality_type = mediaQuality1080P;
        media_content_type = mediaContentVideoMP4;
        media_video_codec_type = mediaVideoH264;
        media_audio_codec_type = mediaAudioAAC;
        break;
    case 22:
        media_quality_type = mediaQuality720P;
        media_content_type = mediaContentVideoMP4;
        media_video_codec_type = mediaVideoH264;
        media_audio_codec_type = mediaAudioAAC;
        break;
    case 35:
        media_quality_type = mediaQuality480P;
        media_content_type = mediaContentVideoFlv;
        media_video_codec_type = mediaVideoH264;
        media_audio_codec_type = mediaAudioAAC;
        break;
    case 34:
        media_quality_type = mediaQuality360P;
        media_content_type = mediaContentVideoFlv;
        media_video_codec_type = mediaVideoH264;
        media_audio_codec_type = mediaAudioAAC;
        break;
    case 18:
        media_quality_type = mediaQuality360P;
        media_content_type = mediaContentVideoMP4;
        media_video_codec_type = mediaVideoH264;
        media_audio_codec_type = mediaAudioAAC;
        break;
    case 5:
        media_quality_type = mediaQuality240P;
        media_content_type = mediaContentVideoFlv;
        media_video_codec_type = mediaVideoH263;
        media_audio_codec_type = mediaAudioMP3;
        break;
    default:
        break;
    }
}

bool is_contain_playlist_impl(const std::string & Url)
{
    return boost::algorithm::contains(Url, "list=");
}

bool parse_playlist_ids(const std::string & content, std::vector< std::string > & urlsArray)
{
    std::string playlist;
    boost::regex re("<div[\\s]+id[\\s]*=[\\s]*\"playlist-bar\".*?data-video-ids[\\s]*=[\\s]*\"([^\"]+)\"");
    boost::sregex_token_iterator i(content.begin(), content.end(), re, 1);
    boost::sregex_token_iterator j;
    if (i != j)
    {
        playlist = (*i++).str();
    }
    else
        return false;

    boost::algorithm::split(urlsArray, playlist, boost::algorithm::is_any_of(","));
    return (!urlsArray.empty());
}


} } } 

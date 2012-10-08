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


/// \file   DTDownloaderYoutubeDetails.cpp

#include "DTHeadersVideoDownload.h"

#include "DTDownloaderYoutubeDetails.h"
#include "DTDownloaderUtils.h"
#include <openmedia/DTMediaDownloader.h>

#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

namespace openmedia { namespace downloader { namespace youtube {

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

#define DT_SEARCH_CONTENT(A) searchContent(content.begin(), content.end(), (A) )

std::string regExSearch(const std::string & regEx, const std::string & content)
{
    return DT_SEARCH_CONTENT(regEx);
}

void split_youtube_video_info3(const std::string & videoInfo, std::vector<std::string> & urls, std::vector<int> &Ids);

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
    propValue = regExSearch(std::string("'PLAYER_CONFIG'\\s*:\\s*[\\{].*\"") +  propname + "\"\\s*:\\s*\"([^\"]*)\"", content);
    return !propValue.empty();
}

bool get_youtube_property_impl2(const std::string & content, const char * propname, std::string & propValue)
{
    propValue = regExSearch(std::string("\"") +  propname + "\"\\s*:\\s*\"([^\"]*)\"", content);
    return !propValue.empty();
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
    propValue = regExSearch(std::string("'PLAYER_CONFIG'\\s*:\\s*[\\{].*\"") +  propname + "\"\\s*:\\s*([\\d]*)", content);
    return !propValue.empty();
}

bool get_youtube_property_d_impl2(const std::string & content, const char * propname, std::string & propValue)
{
    propValue = regExSearch(std::string("\"") +  propname + "\"\\s*:\\s*([\\d]*)", content);
    return !propValue.empty();
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
    if ( !(value = regExSearch("watch\\?v=([^\"&\\s\\\\]+)", content)).empty() )
    {
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

        std::string res = regExSearch("/user/.+/([-_a-zA-Z0-9]+)", url);

        if (!res.empty())
        {
            url = "http://www.youtube.com/watch?v=" + res;
        } 
        else
        {
            res = regExSearch("/user/.+v=([-_a-zA-Z0-9]+)", url);
            if (!res.empty())
            {
                url = "http://www.youtube.com/watch?v=" + res;
            }
        }
    }

    return url;
}

std::string remove_last_comma(const std::string & Url)
{
    std::vector<std::string> resultVal;

    std::string res = regExSearch("([^,]*)[,][\\d]+\\z", Url);
    while (!res.empty())
    {
        return res;
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
    std::string res = regExSearch("<meta[\\s]+name[\\s]*=[\\s]*\"title\"[\\s]+content[\\s]*=[\\s]*\"([^\"]*)\"", content);
    if (!res.empty())
    {
        Title = res;
        return true;
    }
    else 
        return false;    
}

//    <meta name="title" content="Mortal Kombat: New Fatalities Highlights">
void extract_youtube_title(const std::string & content, std::string & Title)
{
    std::string tmpTitle;
    if (! (parse_title_from_metatag(content, tmpTitle) ||
        get_youtube_property(content, "title", tmpTitle)) )
        return;

    Title = replace_html_codes(tmpTitle);
    replace_xml_esc(Title);
    return;
}

bool parse_thumb_url_from_metatag(const std::string & content, std::string & ThumbUrl)
{
    std::string res = regExSearch("<meta[\\s]+property[\\s]*=[\\s]*\"og:image\"[\\s]+content[\\s]*=[\\s]*\"([^\"]*)\"", content);
    if (!res.empty())
    {
        ThumbUrl = res;
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
    //split_youtube_video_info3(videoInfo, urls, Ids);
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

std::string extract_signature(const std::string & content)
{
    std::string sig = regExSearch("signature=([0-9A-Za-z]+\\.[0-9A-Za-z]+)", content);
    if (sig.empty())
        sig = regExSearch("sig=([0-9A-Za-z]+\\.[0-9A-Za-z]+)", content);
    if (sig.empty())
        sig = regExSearch("sig%3D([0-9A-Za-z]+\\.[0-9A-Za-z]+)", content);
    return sig;
}

void split_youtube_video_info_impl4(const std::string & videoInfo, std::vector<std::string> & urls, std::vector<int> &ids)
{
    std::vector<std::string> url_array;
    std::vector<int> id_array;

    std::vector<std::string> splitUrls;
    boost::algorithm::split(splitUrls, videoInfo, boost::algorithm::is_any_of(","));
    if (splitUrls.size())
    {
        BOOST_FOREACH(std::string & url, splitUrls)
        {
            std::string resUrl = "";
            int resId = -1;
            
            {
                std::string res = regExSearch("url=([^&]+)&", url);
                if (!res.empty())
                {
                    resUrl = url_decode(res);
                    std::string sig = extract_signature(url);
                    resUrl += "&signature=" + sig;
                }
            }

            {
                std::string res = regExSearch("itag=([\\d]+)", url);
                if (!res.empty())
                {
                    resId = boost::lexical_cast<int>(res);
                }
            }

            if (!resUrl.empty() && resId >= 0)
            {
                url_array.push_back(resUrl);
                id_array.push_back(resId);        
            }
        }
    }

    ids.swap(id_array);
    urls.swap(url_array);    
}

void split_youtube_video_info3(const std::string & videoInfo, std::vector<std::string> & urls, std::vector<int> &Ids)
{
    if (videoInfo.size())
        split_youtube_video_info_impl4(videoInfo, urls, Ids);

    if (urls.empty())
        split_youtube_video_info_impl3(videoInfo, urls, Ids);
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
        split_youtube_video_info_impl4(fmt_url_map, urls, Ids);

    if (urls.empty())
        split_youtube_video_info_impl3(fmt_url_map, urls, Ids);
}

std::string get_youtube_token(const std::string & content)
{
    std::string token = "";
    get_youtube_property(content, "t", token);
    return token;
}

struct replace_lang_fmt 
{
    std::string operator () (boost::smatch const & what)
    {
        return "hl=en-US";
    }
};

std::string replace_lang(const std::string & input)
{
    if (boost::algorithm::contains(input, "hl="))
        return boost::regex_replace(input, boost::regex( "hl=[^&]+" ), replace_lang_fmt(), boost::match_default | boost::format_all );
    else
        return input + "&hl=en-US";
}

std::string get_transcribed_subtitles(const std::string & content)
{
    std::string url = "";
    get_youtube_property(content, "ttsurl", url);    
    boost::algorithm::replace_all(url, "\\/", "/");
    url = replace_u_codes_4(url);
    url = replace_lang(url);
    url += "&type=track&lang=en&name&kind=asr&fmt=1";
    return url;
}

boost::int64_t get_youtube_duration(const std::string & content)
{
    std::string length = "";
    get_youtube_property_d(content, "length_seconds", length);
    boost::int64_t d = 0;
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
    case 46:
        media_quality_type = mediaQuality1080P;
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
    case 36:
        media_quality_type = mediaQuality240P;
        media_content_type = mediaContentVideo3GP;
        media_video_codec_type = mediaVideoMPEG4;
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
        media_quality_type = mediaQuality240P;
        media_content_type = mediaContentVideoMP4;
        media_video_codec_type = mediaVideoH264;
        media_audio_codec_type = mediaAudioAAC;
        break;
    case 17:
        media_quality_type = mediaQualityQCIF;
        media_content_type = mediaContentVideo3GP;
        media_video_codec_type = mediaVideoMPEG4;
        media_audio_codec_type = mediaAudioAAC;
        break;
    case 5:
        media_quality_type = mediaQuality240P;
        media_content_type = mediaContentVideoFlv;
        media_video_codec_type = mediaVideoH263;
        media_audio_codec_type = mediaAudioMP3;
        break;
    case 84:
        media_quality_type = mediaQuality720P_1;
        media_content_type = mediaContentVideoMP4;
        media_video_codec_type = mediaVideoH264;
        media_audio_codec_type = mediaAudioAAC;
        break;
    case 82:
        media_quality_type = mediaQuality360P;
        media_content_type = mediaContentVideoMP4;
        media_video_codec_type = mediaVideoH264;
        media_audio_codec_type = mediaAudioAAC;
        break;
    case 102:
        media_quality_type = mediaQuality720P_1;
        media_content_type = mediaContentVideoWebm;
        media_video_codec_type = mediaVideoVP8;
        media_audio_codec_type = mediaAudioVorbis;
        break;
    case 100:
        media_quality_type = mediaQuality360P_1;
        media_content_type = mediaContentVideoWebm;
        media_video_codec_type = mediaVideoVP8;
        media_audio_codec_type = mediaAudioVorbis;
        break;
    default:
        std::cout << "Unk : " << Id << "\n"; 
        break;
    }
}

bool is_contain_playlist_impl(const std::string & Url)
{
    return boost::algorithm::contains(Url, "list=");
}

bool parse_playlist_ids(const std::string & content, std::vector< std::string > & url_array)
{
    std::string playlist;
    std::string res = regExSearch("<div[\\s]+id[\\s]*=[\\s]*\"playlist-bar\".*?data-video-ids[\\s]*=[\\s]*\"([^\"]+)\"", content);
    if (!res.empty())
    {
        playlist = res;
    }
    else
        return false;

    boost::algorithm::split(url_array, playlist, boost::algorithm::is_any_of(","));
 /*   BOOST_FOREACH(std::string & elm, urlsArray)
    {
        elm = "http://www.youtube.com/watch?v=" + elm;
    }*/

    return (!url_array.empty());
}


} } } 

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


#ifndef _DT_DOWNLOADERYOUTUBEDETAILS_H_
#define _DT_DOWNLOADERYOUTUBEDETAILS_H_

#include <vector>
#include <string>
#include <openmedia/DTMediaDownloader.h>

namespace openmedia { namespace downloader { namespace youtube {

bool get_youtube_property(const std::string & content, const char * propname, std::string & propValue);
bool get_youtube_property_impl1(const std::string & content, const char * propname, std::string & propValue);
bool get_youtube_property_impl2(const std::string & content, const char * propname, std::string & propValue);

bool get_youtube_property_d(const std::string & content, const char * propname, std::string & propValue);
bool get_youtube_property_d_impl1(const std::string & content, const char * propname, std::string & propValue);
bool get_youtube_property_d_impl2(const std::string & content, const char * propname, std::string & propValue);

void split_youtube_video_info(const std::string & videoInfo, std::vector<std::string> & urls, std::vector<int> &Ids);
void split_youtube_video_info2(const std::string & videoInfo, std::vector<std::string> & urls, std::vector<int> &Ids);

std::string replace_html_codes(const std::string & input);

void extract_youtube_title(const std::string & content, std::string & Title);

std::string extract_youtube_thumbnail_url(const std::string & content);

std::string get_youtube_token(const std::string & content);

std::string get_youtube_video_id(const std::string & content);
std::string regExSearch(const std::string & regEx, const std::string & content);
std::string extract_signature(const std::string & content);

std::string get_transcribed_subtitles(const std::string & content);
boost::int64_t get_youtube_duration(const std::string & content);

void youtube_format_map(int Id, 
                        media_quality_type_t & media_quality_type, 
                        media_content_type_t & media_content_type,
                        media_video_type_t & media_video_codec_type, 
                        media_audio_type_t & media_audio_codec_type);

std::string normalize_youtube_url(const std::string & Url);
bool search_watch_url(const std::string & content, std::string & value);

bool is_contain_playlist_impl(const std::string & Url);

bool parse_playlist_ids(const std::string & content, std::vector< std::string > & urlsArray);

} } }

#endif // _DT_DOWNLOADERYOUTUBEDETAILS_H_

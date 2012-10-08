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


#ifndef _OPENMEDIA_DT_SUBTITLE_YOUTUBE_H_497AB37C
#define _OPENMEDIA_DT_SUBTITLE_YOUTUBE_H_497AB37C

// example:  for video http://www.youtube.com/watch?v=HePypjTZIzY
// subtitle list: http://video.google.com/timedtext?type=list&v=HePypjTZIzY
// get subtitle: http://video.google.com/timedtext?type=track&name=&lang=ru&v=HePypjTZIzY

// see also http://sourceforge.net/projects/google2srt/

#include "DTSubtitle.h"
#include "DTSubtitleDownload.h"

namespace openmedia {
namespace downloader {
namespace youtube {

std::vector<subtitle_track> get_subtitle_list_by_id(const std::string & youTubeId);
void get_subtitles(const std::string & url, std::vector<transcript_element> & result);

} // namespace youtube

class subtitles_youtube : public subtitles
{
public:
    static subtitles_ptr create(const std::string & youtubeId, const std::string & transcribed_url)
    {
        return subtitles_ptr(new subtitles_youtube(youtubeId, transcribed_url));
    }
    subtitles_youtube(const std::string & youtubeId, const std::string & transcribed_url);
};

}
}

#endif

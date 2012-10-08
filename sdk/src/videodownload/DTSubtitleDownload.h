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



#ifndef _OPENMEDIA_DT_SUBTITLE_DOWNLOAD_H_A78E4B72
#define _OPENMEDIA_DT_SUBTITLE_DOWNLOAD_H_A78E4B72

#include <vector>
#include <string>

namespace openmedia {
namespace downloader {

struct subtitle_track 
{
    std::string id; 
    std::string name;
    std::string lang_code; // for example, en
    std::string lang_original; // English
    std::string lang_translated; // English
    bool lang_default; 

    std::string url;
};

struct transcript_element
{
    double start;
    double duration;
    std::string text;
};

void subtitles_to_srt(const std::vector<transcript_element> & subtitles, std::ostream & strm);

}
}

#endif

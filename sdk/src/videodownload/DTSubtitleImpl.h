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


#ifndef _OPENMEDIA_DT_SUBTITLE_IMPL_H_8E906E47
#define _OPENMEDIA_DT_SUBTITLE_IMPL_H_8E906E47

#include <vector>
#include "DTSubtitleDownload.h"

namespace openmedia {
namespace downloader {

class subtitles::Impl
{
public:
    virtual std::vector<subtitle_track> & tracks() = 0;
    virtual void download_subtitle(size_t index, std::vector<transcript_element> & result) = 0;
    virtual ~Impl() = 0;
};


inline subtitles::Impl::~Impl()
{
}

}
}

#endif

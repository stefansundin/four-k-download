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


#ifndef _OPENMEDIA_DT_SLIDESHOW_SOURCE_IMPL_H_INCLUDED_4ED84B5C
#define _OPENMEDIA_DT_SLIDESHOW_SOURCE_IMPL_H_INCLUDED_4ED84B5C

#ifdef _MSC_VER
#pragma once
#endif

#include <openmedia/DTAVSource.h>

namespace openmedia {

class av_source::Impl 
{
public:
    virtual video_source * video() = 0;
    virtual audio_source * audio() = 0;
    virtual double seek(double position) = 0;
    virtual void apply_settings(media_settings_ptr settings) = 0;
    virtual ~Impl() = 0;
};

inline av_source::Impl::~Impl()
{}

}

#endif

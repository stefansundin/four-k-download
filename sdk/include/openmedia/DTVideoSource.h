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


#ifndef _OPENMEDIA_DT_VIDEO_SOURCE_H_INCLUDED_985218EF
#define _OPENMEDIA_DT_VIDEO_SOURCE_H_INCLUDED_985218EF

#ifdef _MSC_VER
#pragma once
#endif

#include <boost/shared_ptr.hpp>
#include <openmedia/DTMediaSource.h>

namespace openmedia {

class video_data;
template <class MediaData> class media_data_timed;
typedef media_data_timed<video_data> video_data_timed;
typedef boost::shared_ptr<video_data_timed> video_data_timed_ptr;

class video_format;
typedef boost::shared_ptr<video_format> video_format_ptr;

/// \class  video_source
class video_source : public media_source
{
public:
    virtual video_data_timed_ptr read_video() = 0;
    virtual video_format_ptr get_video_format() const = 0;
    virtual double get_frame_rate() const = 0;
};

} // namespace openmedia

#endif

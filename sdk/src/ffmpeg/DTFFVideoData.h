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


/// \file   DTFFVideoData.h

#ifndef _DTFFVIDEODATA_H_INCLUDED_
#define _DTFFVIDEODATA_H_INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/shared_ptr.hpp>

#include <openmedia/DTCommon.h>
#include <openmedia/DTVideoData.h>

struct AVPicture;
struct AVFrame;

namespace openmedia {

typedef boost::shared_ptr<AVPicture> AVPicturePtr;
typedef boost::shared_ptr<AVFrame> AVFramePtr;

class ff_video_data;
typedef boost::shared_ptr<ff_video_data> av_video_data_ptr;

class ff_video_data : public video_data
{
public:
    ff_video_data(AVFramePtr _AVFrame, int _Width, int _Height, dt_pixel_format_t _PixelFormat);
    ff_video_data(const video_data * _VideoData);
    ff_video_data(const video_data::Impl * _VideoDataImpl);
};


} // namespace openmedia 

#endif // #ifndef _DTFFVIDEODATA_H_INCLUDED_

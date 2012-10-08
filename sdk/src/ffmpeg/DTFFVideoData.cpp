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



// precompiled header begin
#include "DTHeadersMedia.h"
// precompiled header end

/// \file   DTFFVideoData.cpp

#include "DTFFVideoData.h"
#include "DTFFHeader.h"

#include <openmedia/DTCommon.h>
#include <openmedia/DTError.h>
#include "../DTVideoDataImpl.h"
#include "DTFFVideoDataImpl.h"

namespace openmedia {

ff_video_data::ff_video_data(AVFramePtr _AVFrame, int _Width, int _Height, dt_pixel_format_t _PixelFormat) :
video_data(new ff_video_data_impl(_AVFrame, _Width, _Height, _PixelFormat))
{
}

ff_video_data::ff_video_data(const video_data * _VideoData): video_data( new ff_video_data_impl(_VideoData) )
{
}

ff_video_data::ff_video_data(const video_data::Impl * _VideoDataImpl) : video_data( new ff_video_data_impl(_VideoDataImpl) )
{
}

} // namespace openmedia

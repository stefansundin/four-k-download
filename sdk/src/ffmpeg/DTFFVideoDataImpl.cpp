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

/// \file   ffvideodata.cpp

#include "DTFFVideoDataImpl.h"
#include "DTFFHeader.h"

namespace openmedia {

ff_video_data_impl::ff_video_data_impl(AVFramePtr _AVFrame, int _Width, int _Height, dt_pixel_format_t _PixelFormat):
    m_AVFrame(_AVFrame),
    m_Width(_Width),
    m_Height(_Height),
    m_PixelFormat(_PixelFormat),
    m_PlanesCount(4)
{
    m_DataSize = avpicture_get_size(DT2FFType(_PixelFormat), _Width, _Height);
}

uint8_t * ff_video_data_impl::get_data() const
{
    return m_AVFrame->data[0];
}

uint8_t * ff_video_data_impl::get_plane(size_t _PlaneIndex) const
{
    DT_STRONG_ASSERT(_PlaneIndex < AV_NUM_DATA_POINTERS);
    return m_AVFrame->data[_PlaneIndex];
}

int ff_video_data_impl::get_line_size(size_t _PlaneIndex) const
{
    DT_STRONG_ASSERT(_PlaneIndex < AV_NUM_DATA_POINTERS);
    return m_AVFrame->linesize[_PlaneIndex];
}

} // namespace openmedia

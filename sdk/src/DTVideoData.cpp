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

/// \file   DTVideoData.cpp

#include <openmedia/DTVideoData.h>
#include "DTVideoDataImpl.h"

namespace openmedia {

int video_data::get_width() const
{
    return impl()->get_width();
}

int video_data::get_height() const
{
    return impl()->get_height();
}

dt_pixel_format_t video_data::get_pixel_format() const
{
    return impl()->get_pixel_format();
}

uint8_t * video_data::get_data() const
{
    return impl()->get_data();
}

size_t video_data::get_planes_count() const
{
    return impl()->get_planes_count();
}

uint8_t * video_data::get_plane(size_t _PlaneIndex) const
{
    return impl()->get_plane(_PlaneIndex);
}

int video_data::get_line_size(size_t _PlaneIndex) const
{
    return impl()->get_line_size(_PlaneIndex);
}

video_data_ptr video_data::duplicate() const
{
    return impl()->duplicate();
}

video_data_ptr video_data::clone() const
{
    return this->duplicate();
}

video_data::video_data(video_data::Impl * _Impl)
{
    DT_ASSERT(NULL != _Impl);
    m_pImpl = _Impl;
}

video_data::~video_data()
{
    delete m_pImpl;
}

size_t video_data::get_data_size() const
{
    return impl()->get_data_size();
}

} // namespace openmedia

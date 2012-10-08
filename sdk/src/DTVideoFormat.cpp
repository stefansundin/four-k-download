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

/// \file   DTVideoFormat.cpp
#include <openmedia/DTVideoFormat.h>

namespace openmedia {

class video_format::Impl
{
public:
    Impl(int Width, int Height, dt_pixel_format_t PixFmt, dt_rational_t PixAspectRatio) : width_(Width),
        height_(Height),
        pix_fmt_(PixFmt),
        pix_aspect_ratio_(PixAspectRatio)
    {}

public:
    int                 get_width() const { return width_; }
    int                 get_height() const { return height_; }
    dt_pixel_format_t   get_pix_fmt() const { return pix_fmt_; }
    dt_rational_t       get_pix_aspect_ratio() const { return pix_aspect_ratio_; }

private:
    int width_;
    int height_;
    dt_pixel_format_t pix_fmt_;
    dt_rational_t pix_aspect_ratio_;

};

int video_format::get_width() const
{
    return impl()->get_width();
}

int video_format::get_height() const
{
    return impl()->get_height();
}

dt_pixel_format_t video_format::get_pix_fmt() const
{
    return impl()->get_pix_fmt();
}

dt_rational_t video_format::get_pix_aspect_ratio() const
{
    return impl()->get_pix_aspect_ratio();
}

video_format::video_format(Impl * _Impl) : m_pImpl(_Impl)
{
    DT_STRONG_ASSERT(NULL != _Impl);
}

video_format::~video_format()
{
    delete m_pImpl;
}

video_format_ptr video_format::create(int Width, int Height, dt_pixel_format_t PixFmt)
{
    dt_rational_t aspectRatio = dt_rational_t(1, 1);
    return video_format_ptr( new video_format( new video_format::Impl(Width, Height, PixFmt, aspectRatio) ) );    
}

video_format_ptr video_format::create(int Width, int Height, dt_pixel_format_t PixFmt, dt_rational_t PixAspectRatio)
{
    return video_format_ptr( new video_format( new video_format::Impl(Width, Height, PixFmt, PixAspectRatio) ) );        
}

video_format_ptr video_format::duplicate() const
{
    return video_format_ptr( new video_format( new video_format::Impl(
        this->get_width(), 
        this->get_height(), 
        this->get_pix_fmt(),
        this->get_pix_aspect_ratio()
        )));
}

video_format_ptr video_format::clone() const
{
    return this->duplicate();
}

} // namespace openmedia
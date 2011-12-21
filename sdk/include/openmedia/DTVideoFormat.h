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


/// \file DTVideoFormat.h

#ifndef _DTVIDEOFORMAT_H_INCLUDED_
#define _DTVIDEOFORMAT_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/shared_ptr.hpp>

#include <openmedia/DTCommon.h>
#include <openmedia/DTDeclareImpl.h>
#include <openmedia/DTTypes.h>
#include <openmedia/DTPixFormatTypes.h>

namespace openmedia {

class video_format;
typedef boost::shared_ptr<video_format> video_format_ptr;

/// \class  video_format
class video_format : interface_base
{
    DT_DECLARE_IMPL2(video_format);
public:
    int                 get_width() const;
    int                 get_height() const;
    dt_pixel_format_t   get_pix_fmt() const;
    dt_rational_t       get_pix_aspect_ratio() const;
    virtual             ~video_format();

    video_format_ptr    duplicate() const;
    video_format_ptr    clone() const;

public:
    static video_format_ptr    create(int Width, int Height, dt_pixel_format_t PixFmt);
    static video_format_ptr    create(int Width, int Height, dt_pixel_format_t PixFmt, dt_rational_t AspectRatio);

protected:
    video_format(Impl * _Impl);

private:
    Impl * m_pImpl;
};

inline bool is_same_format(const video_format_ptr & LFmt, video_format_ptr & RFmt)
{
    return ( 
        LFmt->get_width() == RFmt->get_width() &&
        LFmt->get_height() == RFmt->get_height() &&
        LFmt->get_pix_fmt() == RFmt->get_pix_fmt() &&
        LFmt->get_pix_aspect_ratio() == RFmt->get_pix_aspect_ratio()
        );
}

}

#endif //#ifndef _DTVIDEOFORMAT_H_INCLUDED_

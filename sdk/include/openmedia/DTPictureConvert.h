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


/// \file   DTPictureConvert.h

#ifndef _DTPICTURECONVERT_H_INCLUDED_
#define _DTPICTURECONVERT_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <openmedia/DTDeclareImpl.h>
#include <openmedia/DTCommon.h>
#include <openmedia/DTError.h>
#include <openmedia/DTPixFormatTypes.h>

namespace openmedia {

static const int DT_SWS_FAST_BILINEAR   = 1;
static const int DT_SWS_BILINEAR        = 2;
static const int DT_SWS_BICUBIC         = 4;
static const int DT_SWS_X               = 8;
static const int DT_SWS_POINT        = 0x10;
static const int DT_SWS_AREA         = 0x20;
static const int DT_SWS_BICUBLIN     = 0x40;
static const int DT_SWS_GAUSS        = 0x80;
static const int DT_SWS_SINC        = 0x100;
static const int DT_SWS_LANCZOS     = 0x200;
static const int DT_SWS_SPLINE      = 0x400;

class picture_convert;
typedef boost::shared_ptr<picture_convert> picture_convert_ptr;

class video_data;
typedef boost::shared_ptr<video_data> video_data_ptr;

/// \class  picture_convert
class _OPENMEDIASDK_API picture_convert : interface_base
{
    DT_DECLARE_IMPL2(picture_convert);
public:
    video_data_ptr convert(video_data_ptr _InputData);
    virtual ~picture_convert();

public:
    struct error: virtual errors::dt_error {};

protected:
    picture_convert(Impl * _Impl);

private:
    Impl * m_pImpl;
    
};

class _OPENMEDIASDK_API picture_convert_creator
{
public:
    static picture_convert_ptr create(int inWidth, int inHeight, dt_pixel_format_t inPixFormat,
        int outWidth, int outHeight, dt_pixel_format_t outPixelFormat, int flags);
};

} // namespace openmedia

#endif

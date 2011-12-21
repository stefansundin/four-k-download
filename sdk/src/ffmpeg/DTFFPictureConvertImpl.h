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


/// \file   DTFFPictureConvertImpl.h

#ifndef _DTFFPICTURECONVERTIMPL_H_INCLUDED_
#define _DTFFPICTURECONVERTIMPL_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <openmedia/DTCommon.h>
#include <openmedia/DTPictureConvert.h>
#include <openmedia/DTPixFormatTypes.h>
#include "DTFFHeader.h"

#include "../DTPictureConvertImpl.h"

struct AVFrame;
typedef boost::shared_ptr<AVFrame> AVFramePtr;

struct SwsContext;
typedef boost::shared_ptr<SwsContext> SwsContextPtr;

namespace openmedia {

class swscale_picture_convert_impl : public picture_convert::Impl
{
public:
swscale_picture_convert_impl(int _InputWidth, int _InputHeight, dt_pixel_format_t _InputPixelFormat,
        int _OutputWidth, int _OutputHeight, dt_pixel_format_t _OutputPixelFormat,
        int _Flags);

public:
    virtual video_data_ptr convert(video_data_ptr _InputData);

private:
    int outputWidth_;
    int outputHeight_;
    PixelFormat outputPixelFormat_;

    int inputWidth_;
    int inputHeight_;
    PixelFormat inputPixelFormat_;

    SwsContextPtr swsContext_;

    AVFramePtr outputFrame_;

};

} // namespace openmedia

#endif // #ifndef _DTFFPICTURECONVERT_H_INCLUDED_

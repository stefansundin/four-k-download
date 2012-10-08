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

#include <boost/shared_ptr.hpp>

#include <openmedia/DTCommon.h>
#include <openmedia/DTError.h>
#include "DTFFPicture.h"
#include "DTFFHeader.h"
#include <openmedia/DTAssert.h>

namespace openmedia {
namespace details {

class _AVPictureDestroy
{
public:
    void operator()(AVFrame* _AVFrame);
};

class _AVFrameDestroy
{
public:
    void operator()(AVFrame* _AVFrame);
};

void _AVPictureDestroy::operator()(AVFrame* _AVFrame)
{
    if (_AVFrame)
    {
        if (((AVPicture*)_AVFrame)->data[0])
            avpicture_free((AVPicture*)_AVFrame);
        memset(((AVPicture*)_AVFrame)->data, 0, sizeof(((AVPicture*)_AVFrame)->data));
        memset(((AVPicture*)_AVFrame)->linesize, 0, sizeof(((AVPicture*)_AVFrame)->linesize));
        av_free(_AVFrame);
    }
}

void _AVFrameDestroy::operator()(AVFrame* _AVFrame)
{
    if (_AVFrame)
    {
        av_free(_AVFrame);
    }
}

} // namespace details

AVFramePtr dt_create_picture(dt_pixel_format_t _PixelFormat, int _Width, int _Height)
{
    AVFramePtr frame(avcodec_alloc_frame(), details::_AVPictureDestroy());
    if (NULL == frame.get())
        BOOST_THROW_EXCEPTION(errors::bad_alloc());
    
    int dt_err = avpicture_alloc((AVPicture*)frame.get(), DT2FFType(_PixelFormat), _Width, _Height);
    FF_CHECK_ERR(dt_err);
    DT_ASSERT(NULL != frame.get()->data[0]);
    if (FF_ERR(dt_err))
        DT_THROW_AVERROR(errors::unexpected(), dt_err, "avpicture_alloc"); // TODO:?

    return frame;
}

AVFramePtr dt_create_decoded_frame()
{
    AVFramePtr frame(avcodec_alloc_frame(), details::_AVFrameDestroy());
    if (NULL == frame.get())
    {
        BOOST_THROW_EXCEPTION(errors::bad_alloc());
        DT_IF_DISABLE_EXCEPTIONS(return AVFramePtr());
    }
    return frame;
}

} // namespace openmedia

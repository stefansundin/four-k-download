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


/// \file  DTFFVideoDataImpl.h

#ifndef _DTFFVIDEODATAIMPL_H_INCLUDED_
#define _DTFFVIDEODATAIMPL_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <openmedia/DTCommon.h>
#include <openmedia/DTTypes.h>
#include "DTFFHeader.h"
#include "DTFFPicture.h"

#include "../DTVideoDataImpl.h"
#include "DTFFVideoData.h"

struct AVPicture;
struct AVFrame;
typedef boost::shared_ptr<AVPicture> AVPicturePtr;
typedef boost::shared_ptr<AVFrame> AVFramePtr;

namespace openmedia {

/// \class  ff_video_data_impl
class ff_video_data_impl: public video_data::Impl
{
public:
    ff_video_data_impl(AVFramePtr _AVFrame, int _Width, int _Height, dt_pixel_format_t _PixelFormat);
    
    template <class VideoDataClass>
    ff_video_data_impl(const VideoDataClass * _VideoData);

public:
    virtual int                 get_width() const;
    virtual int                 get_height() const;
    virtual dt_pixel_format_t   get_pixel_format() const;
    virtual uint8_t *           get_data() const;
    virtual size_t              get_data_size() const;

    virtual size_t              get_planes_count() const;
    virtual uint8_t *           get_plane(size_t _PlaneIndex) const;
    virtual int                 get_line_size(size_t _PlaneIndex) const;

private:
    AVFramePtr          m_AVFrame;
    int                 m_Width;
    int                 m_Height;
    dt_pixel_format_t   m_PixelFormat;
    size_t              m_PlanesCount;
    size_t              m_DataSize;
};

template <class VideoDataClass>
inline ff_video_data_impl::ff_video_data_impl(const VideoDataClass * _VideoData)
{
    DT_ASSERT(_VideoData != NULL);

    AVPicture sourcePicture;
    for (size_t i = 0; i < AV_NUM_DATA_POINTERS; ++i)
    {
        sourcePicture.data[i] = _VideoData->get_plane(i);
        sourcePicture.linesize[i] = _VideoData->get_line_size(i);
    }

    m_AVFrame       = dt_create_picture(_VideoData->get_pixel_format(), _VideoData->get_width(), _VideoData->get_height());
    av_picture_copy((AVPicture*)m_AVFrame.get(), &sourcePicture, DT2FFType(_VideoData->get_pixel_format()), _VideoData->get_width(), _VideoData->get_height());
    
    m_DataSize      = avpicture_get_size(DT2FFType(_VideoData->get_pixel_format()), _VideoData->get_width(), _VideoData->get_height());
    m_Height        = _VideoData->get_height();
    m_Width         = _VideoData->get_width();
    m_PixelFormat   = _VideoData->get_pixel_format();
    m_PlanesCount   = _VideoData->get_planes_count();
}

inline size_t ff_video_data_impl::get_data_size() const
{
    return m_DataSize;
}

inline int ff_video_data_impl::get_width() const
{
    return m_Width;
}

inline int ff_video_data_impl::get_height() const
{
    return m_Height;
}

inline dt_pixel_format_t ff_video_data_impl::get_pixel_format() const
{
    return m_PixelFormat;
}

inline size_t ff_video_data_impl::get_planes_count() const
{
    return m_PlanesCount;
}

} // namespace openmedia

#endif

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


/// \file   DTVideoData.h

#ifndef _DTVIDEODATA_H_INCLUDED_
#define _DTVIDEODATA_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <openmedia/DTCommon.h>
#include <openmedia/DTDeclareImpl.h>
#include <openmedia/DTTypes.h>
#include <openmedia/DTPixFormatTypes.h>

namespace openmedia {

class video_data;
typedef boost::shared_ptr<video_data> video_data_ptr;

/// \class  video_data
class _OPENMEDIASDK_API video_data: interface_base
{
    DT_DECLARE_IMPL2(video_data);
public:
    int                 get_width() const;
    int                 get_height() const;
    dt_pixel_format_t   get_pixel_format() const;
    uint8_t *           get_data() const;
    size_t              get_data_size() const;

    size_t              get_planes_count() const;
    uint8_t *           get_plane(size_t _PlaneIndex) const;
    int                 get_line_size(size_t _PlaneIndex) const;

    video_data_ptr      duplicate() const;
    video_data_ptr      clone() const;

    /*
    bool                is_key_frame() const;
    dt_picture_type_t   get_picture_type() const;
    dt_ts_t             get_pts() const;
    int                 get_coded_picture_number() const;
    int                 get_display_picture_number() const;
    dt_pic_quality_t    get_quality() const;
    bool                is_reference() const;
    */  

    virtual ~video_data() = 0;

protected:
    video_data(video_data::Impl * _Impl);

private:
    video_data::Impl * m_pImpl;

};

} // namespace openmedia

#endif // #ifndef _DTVIDEODATA_H_INCLUDED_

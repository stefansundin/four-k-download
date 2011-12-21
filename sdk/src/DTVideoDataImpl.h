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


/// \file   DTVideoDataImpl.h

#ifndef _DTVIDEODATAIMPL_H_INCLUDED_
#define _DTVIDEODATAIMPL_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/noncopyable.hpp>

#include <openmedia/DTConfig.h>
#include <openmedia/DTCommon.h>
#include <openmedia/DTTypes.h>
#include <openmedia/DTVideoData.h>

namespace openmedia {

/// \class  video_data::Impl
class video_data::Impl: public impl_base
{
public:
    virtual int                 get_width() const = 0;
    virtual int                 get_height() const = 0;
    virtual dt_pixel_format_t   get_pixel_format() const = 0;
    virtual uint8_t *           get_data() const = 0;
    virtual size_t              get_data_size() const = 0;

    virtual size_t              get_planes_count() const = 0;
    virtual uint8_t *           get_plane(size_t _PlaneIndex) const = 0;
    virtual int                 get_line_size(size_t _PlaneIndex) const = 0;

    virtual video_data_ptr      duplicate() const;
};

} // namespace openmedia

#endif // #ifndef _DTVIDEODATAIMPL_H_INCLUDED_

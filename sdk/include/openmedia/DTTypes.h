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


/// \file   DTTypes.h

#ifndef _DTTYPES_H_INCLUDED_
#define _DTTYPES_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <string>
#include <map>

#include <openmedia/DTCStdInt.h>

namespace openmedia {

typedef double                              dt_float_t;
typedef int64_t                             dt_duration_t;
typedef int64_t                             dt_ts_t;
typedef int                                 dt_media_data_duration_t;
typedef int                                 dt_data_size_t;
typedef int64_t                             dt_count_t;
typedef int64_t                             dt_filesize_t;
typedef uint32_t                            dt_channels_mask_t;
typedef int64_t                             dt_channel_layout_t;
typedef int                                 dt_picture_type_t;
typedef int                                 dt_pic_quality_t;
typedef double                              dt_msecond_t;

typedef short                               dt_auido_sample_t;

typedef std::map<std::string, std::string>  dt_metadata_t;

struct dt_rational_t
{
    dt_rational_t(): num(0), den(1)
    {
    }
    dt_rational_t(int _Num, int _Den): num(_Num), den(_Den)
    {
    }

    double to_double() const
    {
        return (double)num / den;
    }

    int num; ///< numerator
    int den; ///< denominator
};

inline bool operator == (const dt_rational_t & _Left, const dt_rational_t & _Right)
{
    return _Left.num == _Right.num && _Left.den == _Right.den;
}

/// \TODO remove from here
enum dt_own_buffer_t
{
    bufferRef, 
    bufferGetOwn, 
    bufferAllocNew 
};

}

#endif

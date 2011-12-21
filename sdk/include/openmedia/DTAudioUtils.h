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


/// \file   DTAudioUtils.h

#ifndef _DTAUDIOUTILS_H_INCLUDED_
#define _DTAUDIOUTILS_H_INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <openmedia/DTCommon.h>
#include <openmedia/DTSampleFormatTypes.h>

#include <limits>

namespace openmedia {
namespace utils {
namespace audio {

template <typename T>
inline T bits_to_bytes(T count)
{
    return count / std::numeric_limits<uint8_t>::digits;
}

inline int32_t flt_to_int32(float _FloatVal)
{
    if (_FloatVal >= 1.0)
    {
        return (std::numeric_limits<int32_t>::max)();
    } else if (_FloatVal <= -1.0)
    {
        return (std::numeric_limits<int32_t>::min)();
    } else
    {
        return static_cast<int32_t>(_FloatVal * (std::numeric_limits<int32_t>::max)());
    }
}

inline int sample_fmt_to_bps(dt_sample_format_t _SampleFormat)
{
    switch(_SampleFormat)
    {
    case DT_SAMPLE_FMT_U8:
        return 8;
        break;
    case DT_SAMPLE_FMT_S16:
        return 16;
        break;
    case DT_SAMPLE_FMT_S32:
        return 32;
        break;
    case DT_SAMPLE_FMT_FLT:
        return sizeof(float) * std::numeric_limits<uint8_t>::digits;
        break;
    case DT_SAMPLE_FMT_DBL:
        return sizeof(double) * std::numeric_limits<uint8_t>::digits;
        break;
    default:
    case DT_SAMPLE_FMT_NONE:
        return -1;
        break;
    }
}

inline int silence_value(dt_sample_format_t _SampleFormat)
{
    if (DT_SAMPLE_FMT_U8 == _SampleFormat)
        return 128;
    else
        return 0;
}

}
}
} // namespace openmedia 

#endif

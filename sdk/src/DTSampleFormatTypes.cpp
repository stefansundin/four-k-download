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

#include <openmedia/DTSampleFormatTypes.h>
#include <string>

namespace openmedia {

dt_sample_format_t audio_sample_format(const std::string& id)
{
    return audio_sample_format(id.c_str());
}

std::string audio_sample_format(dt_sample_format_t sampleFormat)
{
    switch(sampleFormat)
    {
    case DT_SAMPLE_FMT_U8:
        return DT_SAMPLE_FMT_U8_STR;
    case DT_SAMPLE_FMT_S16:
        return DT_SAMPLE_FMT_S16_STR;
    case DT_SAMPLE_FMT_S32:
        return DT_SAMPLE_FMT_S32_STR;
    case DT_SAMPLE_FMT_FLT:
        return DT_SAMPLE_FMT_FLT_STR;
    case DT_SAMPLE_FMT_DBL:
        return DT_SAMPLE_FMT_DBL_STR;
    default:
        return "";
    }
}

dt_sample_format_t audio_sample_format(const char * id)
{
    if (!id)
        return DT_SAMPLE_FMT_NONE;

    if (std::string(DT_SAMPLE_FMT_U8_STR) == id)
        return DT_SAMPLE_FMT_U8;
    else if (std::string(DT_SAMPLE_FMT_S16_STR) == id)
        return DT_SAMPLE_FMT_S16;
    else if (std::string(DT_SAMPLE_FMT_S32_STR) == id)
        return DT_SAMPLE_FMT_S32;
    else if (std::string(DT_SAMPLE_FMT_FLT_STR) == id)
        return DT_SAMPLE_FMT_FLT;
    else if (std::string(DT_SAMPLE_FMT_DBL_STR) == id)
        return DT_SAMPLE_FMT_DBL;
    else 
        return DT_SAMPLE_FMT_NONE;
}

}

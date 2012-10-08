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
#include "DTHeadersVideoDownload.h"
// precompiled header end

#include "DTSimpleMediaParser.h"
#include <boost/algorithm/string.hpp>

namespace openmedia { namespace downloader {

double to_flv_double(const char * ptr)
{
    char p[8];
    for (size_t i = 0; i < 8; ++i)
    {
        p[7-i] = *(ptr++);
    }
    return *((double *)(&p[0]));
}

bool flv_video_size(const std::string & content, int & width, int & height)
{
    /// TODO
    return false;
    boost::iterator_range< ::std::string::const_iterator > r = boost::algorithm::find_first(content, "width");
    
    size_t avail = content.size() - (r.end() - content.begin());
    if (avail < 9)
        return false;

    const char * ptr = static_cast<const char * >( &*r.end());
    if (*(ptr++) != 0)
        return false;

    width = static_cast<int>(to_flv_double(ptr));

    r = boost::algorithm::find_first(content, "height");
    avail = content.size() - (r.end() - content.begin());
    if (avail < 9)
        return false;

    ptr = static_cast<const char * >( &*r.end());
    if (*(ptr++) != 0)
        return false;

    height = static_cast<int>(to_flv_double(ptr));
    
    return true;
}

bool mp4_video_size(const std::string & /*content*/, int & /*width*/, int & /*height*/)
{
    return false;
}

bool webm_video_size(const std::string & content, int & width, int & height)
{
    return false;
}

} }
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

#include <boost/make_shared.hpp>
#include <openmedia/DTPictureConvert.h>
#include "./ffmpeg/DTFFPictureConvert.h"

using namespace boost;

namespace openmedia {

picture_convert_ptr picture_convert_creator::create(int inWidth, int inHeight, dt_pixel_format_t inPixFormat,
        int outWidth, int outHeight, dt_pixel_format_t outPixelFormat, int flags)
{
    return make_shared<swscale_picture_convert>(inWidth, inHeight, inPixFormat,
        outWidth, outHeight, outPixelFormat,
        flags);
}

} // namespace openmedia

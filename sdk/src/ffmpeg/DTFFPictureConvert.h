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


/// \file   DTFFPictureConvert.h

#ifndef _DTFFPICTURECONVERT_H_INCLUDED_
#define _DTFFPICTURECONVERT_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <openmedia/DTCommon.h>
#include <openmedia/DTPictureConvert.h>
#include <openmedia/DTPixFormatTypes.h>

namespace openmedia {

class swscale_picture_convert : public picture_convert
{
public:
swscale_picture_convert(int inWidth, int inHeight, dt_pixel_format_t inPixFormat,
        int outWidth, int outHeight, dt_pixel_format_t outPixelFormat, int flags);

};

} // namespace openmedia

#endif // #ifndef _DTFFPICTURECONVERT_H_INCLUDED_

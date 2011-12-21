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




#include <openmedia/DTHeaders.h>


#include "DTFFPictureConvert.h"
#include "DTFFPictureConvertImpl.h"

namespace openmedia {

swscale_picture_convert::swscale_picture_convert(int _InputWidth, int _InputHeight, dt_pixel_format_t _InputPixelFormat,
        int _OutputWidth, int _OutputHeight, dt_pixel_format_t _OutputPixelFormat,
        int _Flags) : 
picture_convert( 
                new swscale_picture_convert_impl(
                            _InputWidth, _InputHeight, _InputPixelFormat,
                            _OutputWidth, _OutputHeight, _OutputPixelFormat,_Flags)
                )
{
}

} // namespace openmedia

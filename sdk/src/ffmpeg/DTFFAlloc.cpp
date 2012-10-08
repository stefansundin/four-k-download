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

/// \file   DTFFDecoderInfo.cpp

#include "DTFFAlloc.h"

#define DT_FF_NEED_AVUTIL
#include "DTFFHeader.h"

#include <openmedia/DTError.h>

namespace openmedia {

void ff_free(uint8_t ** _Mem)
{
    av_freep( (void*)_Mem );
};

void ff_alloc(uint8_t ** _Dst, size_t _Size, size_t _Pad)
{
    if (!_Dst)
        BOOST_THROW_EXCEPTION(errors::invalid_pointer());

    *_Dst = NULL;
    if (NULL != (*_Dst = (uint8_t *)av_malloc(_Size + _Pad)))
    {
        if (_Pad)
            memset((uint8_t *)(*_Dst) + _Size, 0, _Pad);
    }else
        BOOST_THROW_EXCEPTION(errors::bad_alloc());
}

void ff_alloc_and_copy(uint8_t ** _Dst, const uint8_t * _Src, size_t _Size, size_t _Pad)
{
    * _Dst = NULL;
    if (_Src && _Size > 0)
    {
        ff_alloc(_Dst, _Size, _Pad);
        memcpy(*_Dst, _Src, _Size);
    }
}

}

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


/// \file   DTFFAlloc.h

#ifndef _DTFFALLOC_H_INCLUDED_
#define _DTFFALLOC_H_INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <openmedia/DTConfig.h>
#include <openmedia/DTCStdInt.h>

namespace openmedia {

_OPENMEDIASDK_API void ff_alloc_and_copy(uint8_t ** _Dst, const uint8_t * _Src, size_t _Size, size_t _Pad);  
_OPENMEDIASDK_API void ff_alloc(uint8_t ** _Dst, size_t _Size, size_t _Pad);  
_OPENMEDIASDK_API void ff_free(uint8_t ** _Mem);  

}
#endif

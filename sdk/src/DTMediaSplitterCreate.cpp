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


/// \file   DTMediaSplitter.cpp

#include <openmedia/DTCommon.h>
#include <openmedia/DTMediaSplitter.h>

#include "DTMediaSplitterImpl.h"
#include "ffmpeg/DTFFMediaSplitter.h"

namespace openmedia {

media_splitter_ptr media_splitter_creator::create(const char * _Filename)
{
    return media_splitter_ptr( new ff_media_splitter(_Filename) );
}

#if defined(DT_CONFIG_HAVE_UTF16_OPEN) && (1 == DT_CONFIG_HAVE_UTF16_OPEN)
media_splitter_ptr media_splitter_creator::create(const wchar_t * _Filename)
{
    return media_splitter_ptr( new ff_media_splitter(_Filename) );
}
#endif

}

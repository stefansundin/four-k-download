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

/// \file   DTFFFileInfo.cpp

#include "DTFFFileInfo.h"
#include "DTFFFileInfoImpl.h"

#define DT_FF_NEED_AVFORMAT
#include "DTFFHeader.h"

namespace openmedia {

ff_file_info_general::ff_file_info_general(AVFormatContext * _Formatcontext):
file_info_general( new ff_file_info_general_impl(_Formatcontext) ) 
{}

void ff_file_info_general::set(AVFormatContext * _Formatcontext)
{
    this->reset( new ff_file_info_general_impl(_Formatcontext) );
}

} // namespace openmedia

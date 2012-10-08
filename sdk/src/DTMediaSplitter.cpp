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

/// \file   DTMediaSplitter.cpp

#include <openmedia/DTCommon.h>
#include <openmedia/DTMediaSplitter.h>

#include "DTMediaSplitterImpl.h"
#include "ffmpeg/DTFFMediaSplitter.h"

namespace openmedia {

media_packet_ptr media_splitter::read_packet()
{
    return impl()->read_packet();
}

void media_splitter::seek(dt_ts_t _Position)
{
    return impl()->seek(_Position);
}

file_info_general_ptr media_splitter::get_file_info() const
{
    return impl()->get_file_info();
}

stream_info_ptr media_splitter::get_stream_info(unsigned int _StreamIndex) const
{
    return impl()->get_stream_info(_StreamIndex);
}

bool media_splitter::is_eof()
{
    return impl()->is_eof();
}


media_splitter::media_splitter(media_splitter::Impl * _Impl) : m_pImpl(_Impl)
{
    DT_STRONG_ASSERT(NULL != _Impl);
}

media_splitter::~media_splitter()
{
    delete m_pImpl;
}

} // namespace openmedia

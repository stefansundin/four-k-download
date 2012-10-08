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

/// \file   DTMediaMuxer.cpp

#include <openmedia/DTAssert.h>
#include <openmedia/DTMediaMuxer.h>
#include "DTMediaMuxerImpl.h"

namespace openmedia {

void media_muxer::open()
{
    return impl()->open();
}

void media_muxer::write_packet(media_packet_ptr _MediaPacket)
{
    return impl()->write_packet(_MediaPacket);
}

void media_muxer::write_packet(media_packet_ptr _MediaPacket, int stream)
{
    return impl()->write_packet(_MediaPacket, stream);
}

void media_muxer::close()
{
    return impl()->close();
}

media_muxer::media_muxer(media_muxer::Impl * _Impl): m_pImpl(_Impl)
{
    DT_STRONG_ASSERT(NULL != _Impl);
}

void media_muxer::reset_impl(media_muxer::Impl * _Impl)
{
    delete m_pImpl;
    DT_STRONG_ASSERT(NULL != _Impl);
    m_pImpl = _Impl;
}

media_muxer::~media_muxer()
{
    close();
    delete m_pImpl;
}

} // namespace openmedia

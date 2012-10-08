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

/// \file   DTPacket.cpp

#include <openmedia/DTPacket.h>
#include <openmedia/DTAssert.h>
#include "DTPacketImpl.h"

namespace openmedia {

dt_ts_t media_packet::get_pts() const
{
    return impl()->get_pts();
}

dt_ts_t media_packet::get_dts() const
{
    return impl()->get_dts();
}

const uint8_t * media_packet::get_data() const
{
    return impl()->get_data();
}

dt_data_size_t media_packet::get_size() const
{
    return impl()->get_size();
}

int media_packet::get_stream_index() const
{
    return impl()->get_stream_index();
}

int media_packet::get_flags() const
{
    return impl()->get_flags();
}

dt_media_data_duration_t media_packet::get_duration() const
{
    return impl()->get_duration();
}

dt_filesize_t media_packet::get_byte_pos() const
{
    return impl()->get_byte_pos();
}

dt_media_type_t media_packet::get_media_type() const
{
    return impl()->get_media_type();
}

media_packet::media_packet(media_packet::Impl * _Impl) : m_pImpl(_Impl)
{
    DT_STRONG_ASSERT(NULL != _Impl);
}

media_packet::~media_packet()
{
    delete m_pImpl;
}

bool media_packet::is_valid() const
{
    return impl()->is_valid();
}

media_packet_ptr media_packet::duplicate() const
{
    return impl()->duplicate();
}

media_packet_ptr media_packet::clone() const
{
    return this->duplicate();
}

dt_rational_t media_packet::get_time_base() const
{
    return impl()->get_time_base();
}

dt_rational_t media_packet::get_frame_rate() const
{
    return impl()->get_frame_rate();
}

} // namespace openmedia 

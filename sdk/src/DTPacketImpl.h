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


/// \file   DTPacketImpl.h

#ifndef _DTPACKETIMPL_H_INCLUDED_
#define _DTPACKETIMPL_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/noncopyable.hpp>

#include <openmedia/DTCStdInt.h>
#include <openmedia/DTTypes.h>
#include <openmedia/DTMediaTypes.h>
#include <openmedia/DTPacket.h>

namespace openmedia {

class media_packet::Impl: public impl_base
{
public:
    virtual dt_ts_t             get_pts() const = 0;
    virtual dt_ts_t             get_dts() const = 0;
    virtual const uint8_t *     get_data() const = 0;
    virtual dt_data_size_t      get_size() const = 0;
    virtual int                 get_stream_index() const = 0;
    virtual int                 get_flags() const = 0;
    virtual dt_media_data_duration_t get_duration() const = 0;
    virtual dt_filesize_t       get_byte_pos() const = 0;
    virtual dt_media_type_t     get_media_type() const = 0;
    virtual bool                is_valid() const = 0;
    virtual media_packet_ptr    duplicate() const;
    virtual dt_rational_t       get_time_base() const = 0;
    virtual dt_rational_t       get_frame_rate() const = 0;
};

} // namespace openmedia

#endif

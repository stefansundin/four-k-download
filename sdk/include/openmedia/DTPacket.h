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


/// \file   DTPacket.h

#ifndef _DTPACKET_H_INCLUDED_
#define _DTPACKET_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <openmedia/DTConfig.h>
#include <openmedia/DTCommon.h>
#include <openmedia/DTDeclareImpl.h>
#include <openmedia/DTCStdInt.h>
#include <openmedia/DTTypes.h>
#include <openmedia/DTMediaTypes.h>

namespace openmedia {

class media_packet;
typedef boost::shared_ptr<media_packet> media_packet_ptr;

/// \class  media_packet
class _OPENMEDIASDK_API media_packet : interface_base
{
    DT_DECLARE_IMPL2(media_packet);
public:
    dt_ts_t             get_pts() const;
    dt_ts_t             get_dts() const;
    const uint8_t *     get_data() const;
    dt_data_size_t      get_size() const;
    int                 get_stream_index() const;
    int                 get_flags() const;
    dt_media_data_duration_t get_duration() const;
    dt_filesize_t       get_byte_pos() const;
    dt_media_type_t     get_media_type() const;
    bool                is_valid() const;
    media_packet_ptr    duplicate() const;
    media_packet_ptr    clone() const;
    dt_rational_t       get_time_base() const;
    dt_rational_t       get_frame_rate() const;

    virtual             ~media_packet();

public:
    media_packet(Impl * _Impl);

private:
    media_packet() {};

private:
    Impl * m_pImpl;
};

//  \class  media_packet_utils
class _OPENMEDIASDK_API media_packet_utils
{
public:
    static media_packet_ptr create_null_packet();
    static media_packet_ptr create_empty_packet();
    static bool is_null_packet(const media_packet_ptr & _MediaPacket);
};

} // namespace openmedia

#endif

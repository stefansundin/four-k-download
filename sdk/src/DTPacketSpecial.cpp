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

/// \file DTPacketSpecial.cpp

#include <openmedia/DTPacket.h>
#include "DTPacketImpl.h"

namespace openmedia {

class   null_packet;
static  null_packet * nullPacketPtr;
static  null_packet * emptyPacketPtr;

static const char   specialData[] = "DTSPECIALPACKET";
static const size_t specialDataSize = sizeof(specialData);

/// \class  null_packet_impl
class null_packet_impl: public media_packet::Impl
{
public:
    null_packet_impl(const uint8_t * data, size_t dataSize)  : data_(data), dataSize_(dataSize)
    {
    
    }
private:
    const uint8_t * data_;
    size_t dataSize_;

public:
    virtual dt_ts_t             get_pts() const    {        return 0;    }
    virtual dt_ts_t             get_dts() const    {        return 0;        }
    virtual const uint8_t *     get_data() const   {        return (uint8_t *)data_;    }
    virtual dt_data_size_t      get_size() const    {        return dataSize_;    }
    virtual int                 get_stream_index() const    {        return -1;    }
    virtual int                 get_flags() const    {        return 0;    }
    virtual dt_media_data_duration_t get_duration() const    {        return 0;    }
    virtual dt_filesize_t       get_byte_pos() const    {        return 0;        }
    virtual dt_media_type_t     get_media_type() const    {        return DT_AVMEDIA_TYPE_UNKNOWN;    }
    virtual bool                is_valid() const    {        return true;    }
    virtual media_packet_ptr    duplicate() const    {        return media_packet_utils::create_null_packet();    }
    virtual dt_rational_t       get_time_base() const    {        return dt_rational_t();    }
    virtual dt_rational_t       get_frame_rate() const    {        return dt_rational_t();    }
    virtual ~null_packet_impl()    {    }
};

/// \class  null_packet
class null_packet: public media_packet
{
public:
    null_packet(const uint8_t * data, size_t dataSize): media_packet( new null_packet_impl(data, dataSize) )
    {}
};

namespace {
static null_packet nullPacket(((uint8_t*)(specialData[0])), (specialDataSize));
static null_packet emptyPacket((NULL), (0));
const bool b1 = (bool)(NULL != (nullPacketPtr = &nullPacket));
const bool b2 = (bool)(NULL != (emptyPacketPtr = &emptyPacket));
}

media_packet_ptr media_packet_utils::create_null_packet()
{
    return media_packet_ptr(nullPacketPtr, utils::no_delete());
}

bool media_packet_utils::is_null_packet(const media_packet_ptr & _MediaPacket)
{
    return (_MediaPacket.get() == nullPacketPtr);
}

media_packet_ptr media_packet_utils::create_empty_packet()
{
    return media_packet_ptr(emptyPacketPtr, utils::no_delete());
}

} // namespace openmedia

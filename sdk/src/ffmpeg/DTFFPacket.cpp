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

/// \file   DTFFPacket.cpp

#include "DTFFPacket.h"

#include <openmedia/DTConfig.h>
#include <openmedia/DTError.h>
#include <openmedia/DTPacket.h>

#define DT_FF_NEED_AVFORMAT
#include "DTFFHeader.h"

#include "../DTPacketImpl.h"
#include "../md5/md5.h"

namespace openmedia {

/// \class  ff_media_packet_impl
/// \brief  Media packet implementation based on AVPacket (libavformat)
class ff_media_packet_impl : public media_packet::Impl
{
public:
    ff_media_packet_impl(AVPacketPtr _AVPacket, dt_media_type_t _MediaType, dt_rational_t _TimeBase);
    ff_media_packet_impl(AVPacketPtr _AVPacket, dt_media_type_t _MediaType, dt_rational_t _TimeBase, dt_rational_t frameRate);
    
public:
    virtual dt_ts_t             get_pts() const;
    virtual dt_ts_t             get_dts() const;
    virtual const uint8_t *     get_data() const;
    virtual dt_data_size_t      get_size() const;
    virtual int                 get_stream_index() const;
    virtual int                 get_flags() const;
    virtual dt_media_data_duration_t get_duration() const;
    virtual dt_media_type_t     get_media_type() const;
    virtual dt_filesize_t       get_byte_pos() const;
    virtual bool                is_valid() const;
    virtual dt_rational_t       get_time_base() const;
    virtual dt_rational_t       get_frame_rate() const;

private:
    AVPacketPtr     m_AVPacket;
    bool            m_Valid;
    dt_media_type_t m_MediaType;
    dt_rational_t   m_TimeBase;
    dt_rational_t   m_FrameRate;

private:
#if defined(DT_VALIDATE_PACKET) && (DT_VALIDATE_PACKET == 1)
    md5_byte_t m_MD5[16];
    bool CheckMD5(md5_byte_t _MD5[16]) const;
#endif // defined(DT_VALIDATE_PACKET) && (DT_VALIDATE_PACKET == 1)
};

// ff_media_packet_impl impl

inline ff_media_packet_impl::
ff_media_packet_impl(AVPacketPtr _AVPacket, dt_media_type_t _MediaType, dt_rational_t _TimeBase, dt_rational_t frameRate):
m_AVPacket(_AVPacket),
m_Valid(true),
m_MediaType(_MediaType),
m_TimeBase(_TimeBase),
m_FrameRate(frameRate)
{
#if defined(DT_VALIDATE_PACKET) && (DT_VALIDATE_PACKET == 1)
    md5_state_t md5State;
    md5_init(&md5State);
    md5_append(&md5State, _AVPacket->data, _AVPacket->size);
    md5_finish(&md5State, m_MD5);
#endif // defined(DT_VALIDATE_PACKET) && (DT_VALIDATE_PACKET == 1)
}

inline ff_media_packet_impl::ff_media_packet_impl(AVPacketPtr _AVPacket, dt_media_type_t _MediaType, dt_rational_t _TimeBase) :
m_AVPacket(_AVPacket),
m_Valid(true),
m_MediaType(_MediaType),
m_TimeBase(_TimeBase)

{
#if defined(DT_VALIDATE_PACKET) && (DT_VALIDATE_PACKET == 1)
    md5_state_t md5State;
    md5_init(&md5State);
    md5_append(&md5State, _AVPacket->data, _AVPacket->size);
    md5_finish(&md5State, m_MD5);
#endif // defined(DT_VALIDATE_PACKET) && (DT_VALIDATE_PACKET == 1)
}

#if defined(DT_VALIDATE_PACKET) && (DT_VALIDATE_PACKET == 1)
inline bool ff_media_packet_impl::CheckMD5(md5_byte_t _MD5[16]) const
{
    for (size_t i = 0; i < 16; ++i)
        if (_MD5[i] != m_MD5[i])
        {
            DT_LOG(error) << "validate packet failed";
            return false;
        }
    return true;
}
#endif // defined(DT_VALIDATE_PACKET) && (DT_VALIDATE_PACKET == 1)

inline dt_ts_t ff_media_packet_impl::get_pts() const
{
    return m_AVPacket->pts;
}

inline dt_ts_t ff_media_packet_impl::get_dts() const
{
    return m_AVPacket->dts;
}

inline const uint8_t * ff_media_packet_impl::get_data() const
{
    return m_AVPacket->data;
}

inline dt_data_size_t ff_media_packet_impl::get_size() const
{
    return m_AVPacket->size;
}

inline int ff_media_packet_impl::get_stream_index() const
{
    return m_AVPacket->stream_index;
}

inline int ff_media_packet_impl::get_flags() const
{
    return m_AVPacket->flags;
}

inline dt_media_data_duration_t ff_media_packet_impl::get_duration() const
{
    return m_AVPacket->duration;
}

inline dt_filesize_t ff_media_packet_impl::get_byte_pos() const
{
    return m_AVPacket->pos;
}

inline dt_media_type_t ff_media_packet_impl::get_media_type() const
{
    return m_MediaType;
}

inline bool ff_media_packet_impl::is_valid() const
{
#if defined(DT_VALIDATE_PACKET) && (DT_VALIDATE_PACKET == 1)
    md5_byte_t currentMD5[16];
    md5_state_t md5State;
    md5_init(&md5State);
    md5_append(&md5State, m_AVPacket->data, m_AVPacket->size);
    md5_finish(&md5State, currentMD5);
    bool valid = CheckMD5(currentMD5);
    DT_ALL_ASSERT(valid);
    return valid;
#else // defined(DT_VALIDATE_PACKET) && (DT_VALIDATE_PACKET == 1)
    return m_Valid;
#endif // defined(DT_VALIDATE_PACKET) && (DT_VALIDATE_PACKET == 1)
}

inline dt_rational_t ff_media_packet_impl::get_time_base() const
{
    return m_TimeBase;
}

inline dt_rational_t ff_media_packet_impl::get_frame_rate() const
{
    return m_FrameRate;
}

// ff_media_packet impl

namespace 
{
}

ff_media_packet::ff_media_packet(AVPacketPtr _AVPacket): media_packet(new ff_media_packet_impl(_AVPacket, DT_AVMEDIA_TYPE_UNKNOWN, dt_rational_t()))
{
}

ff_media_packet::ff_media_packet(AVPacketPtr _AVPacket, dt_media_type_t _MediaType):
media_packet(new ff_media_packet_impl(_AVPacket, _MediaType, dt_rational_t()))
{
}

ff_media_packet::ff_media_packet(AVPacketPtr _AVPacket, dt_media_type_t _MediaType, dt_rational_t _TimeBase)
: media_packet(new ff_media_packet_impl(_AVPacket, _MediaType, _TimeBase))
{
}

ff_media_packet::
ff_media_packet(AVPacketPtr _AVPacket, dt_media_type_t _MediaType, dt_rational_t _TimeBase, dt_rational_t frameRate) :
media_packet(new ff_media_packet_impl(_AVPacket, _MediaType, _TimeBase, frameRate))
{
}

} // namespace openmedia

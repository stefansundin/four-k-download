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

/// \file   DTCommonPacket.h

#include <openmedia/DTPacket.h>
#include <openmedia/DTError.h>

//#include <openmedia/DTUtils.h>


//#include <openmedia/log/DTLog.h>

#include "DTPacketImpl.h"
#include "DTCommonPacket.h"


namespace openmedia {

/// \class  common_media_packet_impl
template <class MediaPacketClass>
class common_media_packet_impl: public media_packet::Impl
{
public:
    common_media_packet_impl(const MediaPacketClass * _MediaPacket);
    virtual ~common_media_packet_impl();

public:
    virtual dt_ts_t             get_pts() const;
    virtual dt_ts_t             get_dts() const;
    virtual const uint8_t *     get_data() const;
    virtual dt_data_size_t      get_size() const;
    virtual int                 get_stream_index() const;
    virtual int                 get_flags() const;
    virtual dt_media_data_duration_t get_duration() const;
    virtual dt_filesize_t       get_byte_pos() const;
    virtual dt_media_type_t     get_media_type() const;
    virtual bool                is_valid() const;
    virtual dt_rational_t       get_time_base() const;
    virtual dt_rational_t       get_frame_rate() const;

private:
    dt_ts_t         m_pts;
    dt_ts_t         m_dts;
    uint8_t *       m_data;
    dt_data_size_t  m_size;
    int             m_stream_index;
    int             m_flags;
    dt_media_data_duration_t m_duration;
    dt_filesize_t   m_byte_pos;
    dt_media_type_t m_media_type;
    bool            m_is_valid;
    dt_rational_t   m_time_base;
    dt_rational_t   m_frame_rate;
};

namespace {

// TODO

static const size_t DT_INPUT_BUFFER_PADDING_SIZE = 8;

void dt_free(uint8_t ** _Mem)
{
    if (_Mem && *_Mem)
    {
        free( (void*)(*_Mem) );
        *_Mem = NULL;
    }
};

void dt_alloc(uint8_t ** _Dst, size_t _Size, size_t _Pad)
{
    if (!_Dst)
        BOOST_THROW_EXCEPTION(errors::invalid_pointer());

    *_Dst = NULL;
    if (NULL != (*_Dst = (uint8_t *)malloc(_Size + _Pad)))
    {
        if (_Pad)
            memset((uint8_t *)(*_Dst) + _Size, 0, _Pad);
    }else
        BOOST_THROW_EXCEPTION(errors::bad_alloc());
}

void alloc_and_copy(uint8_t ** _Dst, const uint8_t * _Src, size_t _Size, size_t _Pad)
{
    * _Dst = NULL;
    if (_Src && _Size > 0)
    {
        dt_alloc(_Dst, _Size, _Pad);
        memcpy(*_Dst, _Src, _Size);
    }
}

}

template <class MediaPacketClass>
common_media_packet_impl<MediaPacketClass>::common_media_packet_impl(const MediaPacketClass * _MediaPacket)
{
    DT_STRONG_ASSERT(NULL != _MediaPacket);
    if (NULL == _MediaPacket)
    {
        DT_ASSERT(false);
        BOOST_THROW_EXCEPTION(errors::invalid_pointer());
    }
    
    alloc_and_copy(&m_data, _MediaPacket->get_data(), _MediaPacket->get_size(), DT_INPUT_BUFFER_PADDING_SIZE);
    m_size = _MediaPacket->get_size();

    m_pts           = _MediaPacket->get_pts();
    m_dts           = _MediaPacket->get_dts();
    m_stream_index  = _MediaPacket->get_stream_index();
    m_flags         = _MediaPacket->get_flags();
    m_duration      = _MediaPacket->get_duration();
    m_byte_pos      = _MediaPacket->get_byte_pos();
    m_media_type    = _MediaPacket->get_media_type();
    m_is_valid      = _MediaPacket->is_valid();
    m_time_base     = _MediaPacket->get_time_base();
    m_frame_rate    = _MediaPacket->get_frame_rate();
}

template <class MediaPacketClass>
common_media_packet_impl<MediaPacketClass>::~common_media_packet_impl()
{
    m_is_valid = false;
    dt_free(&m_data);
}

template <class MediaPacketClass>
inline dt_ts_t common_media_packet_impl<MediaPacketClass>::get_pts() const
{
    return m_pts;
}

template <class MediaPacketClass>
inline dt_ts_t common_media_packet_impl<MediaPacketClass>::get_dts() const
{
    return m_dts;
}

template <class MediaPacketClass>
inline const uint8_t * common_media_packet_impl<MediaPacketClass>::get_data() const
{
    return m_data;
}

template <class MediaPacketClass>
inline dt_data_size_t common_media_packet_impl<MediaPacketClass>::get_size() const 
{
    return m_size;
}

template <class MediaPacketClass>
inline int common_media_packet_impl<MediaPacketClass>::get_stream_index() const
{
    return m_stream_index;
}

template <class MediaPacketClass>
inline int common_media_packet_impl<MediaPacketClass>::get_flags() const
{
    return m_flags;
}

template <class MediaPacketClass>
inline dt_media_data_duration_t common_media_packet_impl<MediaPacketClass>::get_duration() const
{
    return m_duration;
}

template <class MediaPacketClass>
inline dt_filesize_t common_media_packet_impl<MediaPacketClass>::get_byte_pos() const
{
    return m_byte_pos;
}

template <class MediaPacketClass>
inline dt_media_type_t common_media_packet_impl<MediaPacketClass>::get_media_type() const
{
    return m_media_type;
}

template <class MediaPacketClass>
inline bool common_media_packet_impl<MediaPacketClass>::is_valid() const
{
    return m_is_valid;
}

template <class MediaPacketClass>
inline dt_rational_t common_media_packet_impl<MediaPacketClass>::get_time_base() const
{
    return m_time_base;
}

template <class MediaPacketClass>
inline dt_rational_t common_media_packet_impl<MediaPacketClass>::get_frame_rate() const
{
    return m_frame_rate;
}

///

common_media_packet::common_media_packet(const media_packet * _MediaPacket): media_packet( new common_media_packet_impl<media_packet>(_MediaPacket) )
{
}

common_media_packet::common_media_packet(const media_packet::Impl * _MediaPacket): media_packet( new common_media_packet_impl<media_packet::Impl>(_MediaPacket) )
{
}

//////////////////////////////////////////////////////////////////////////////

void common_media_packet2_impl::setter::set_data(boost::shared_array<uint8_t> _Buffer, size_t _Size)
{
    m_owner->m_data = _Buffer;
    m_owner->m_size = _Size;
}

namespace {

}

void common_media_packet2_impl::setter::set_data(uint8_t * _Buffer, size_t _Size, dt_own_buffer_t _OwnBuffer)
{
    if (bufferRef == _OwnBuffer)
    {
        set_data(boost::shared_array<uint8_t>(_Buffer, utils::no_delete()), _Size );
    }
    else if (bufferAllocNew == _OwnBuffer)
    {
        DT_ASSERT(NULL != m_owner->m_AllocBuffer);
        if (!m_owner->m_AllocBuffer)
        {
            BOOST_THROW_EXCEPTION(errors::invalid_operation());
        }

        uint8_t * buffer = NULL;
        if (_Size > 0)
        {
            buffer = m_owner->m_AllocBuffer(_Size);
            memcpy(buffer, _Buffer, _Size);
        }
        set_data(boost::shared_array<uint8_t> ( buffer, utils::func_deleter<FreeBufferT>( m_owner->m_FreeBuffer ) ), _Size );
    }
    else if (bufferGetOwn == _OwnBuffer)
    {
        set_data(boost::shared_array<uint8_t> ( _Buffer, utils::func_deleter<FreeBufferT>( m_owner->m_FreeBuffer ) ), _Size);
    }        
}

void common_media_packet2_impl::final_construct()
{
    m_dts = 0;
    m_pts = 0;
    m_time_base = dt_rational_t();
    m_duration = 0;
    m_stream_index = 0;
    m_byte_pos = 0;
    m_media_type = DT_AVMEDIA_TYPE_UNKNOWN;
}

common_media_packet2_impl::common_media_packet2_impl()
{
}

} // namespace openmedia


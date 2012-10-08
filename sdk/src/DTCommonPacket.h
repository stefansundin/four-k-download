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


/// \file   DTCommonPacket.h

#ifndef _DTCOMMONPACKET_H_INCLUDED_
#define _DTCOMMONPACKET_H_INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>

#include <openmedia/DTCStdInt.h>
#include <openmedia/DTTypes.h>
#include <openmedia/DTCommon.h>
#include <openmedia/DTPacket.h>

#include "DTAllocator.h"
#include "DTPacketImpl.h"

namespace openmedia {

/// \class  media_packet_properties
/// \brief  Media Data Packet Set Properties Interface
class _OPENMEDIASDK_API media_packet_properties : interface_base
{
public:
    virtual void set_data(boost::shared_array<uint8_t> _Buffer, size_t _Size) = 0;
    virtual void set_data(uint8_t * _Buffer, size_t _Size, dt_own_buffer_t _OwnBuffer) = 0;
    virtual void set_pts(dt_ts_t _Pts) = 0;
    virtual void set_dts(dt_ts_t _Dts) = 0;
    virtual void set_stream_index(int _StreamIndex) = 0;
    virtual void set_flags(int _Flags) = 0;
    virtual void set_duration(dt_media_data_duration_t _Duration) = 0;
    virtual void set_byte_pos(dt_filesize_t _BytePos) = 0;
    virtual void set_media_type(dt_media_type_t _MediaType) = 0;
    virtual void set_time_base(dt_rational_t _TimeBase) = 0;
    virtual void set_frame_rate(dt_rational_t frameRate) = 0;

protected:
    ~media_packet_properties(){};

};

class common_media_packet;
typedef boost::shared_ptr<common_media_packet> common_media_packet_ptr;

class _OPENMEDIASDK_API common_media_packet : public media_packet
{
public:
    common_media_packet(const media_packet * _MediaPacket);
    common_media_packet(const media_packet::Impl * _MediaPacketImpl);
};

template <class MemoryAllocator>
class _OPENMEDIASDK_API common_media_packet2_t : public media_packet, public MemoryAllocator
{
public:
    common_media_packet2_t();
    media_packet_properties * setter();

private:
    boost::shared_ptr<media_packet_properties> m_properties;
    void final_construct();

};

template <class MemoryAllocator>
inline media_packet_properties * common_media_packet2_t<MemoryAllocator>::setter()
{
    return m_properties.get();
}

//////////////////////////////////////////////////////////////////////////////////////////

typedef common_media_packet2_t < 
                Allocator::DefaultArrayAllocator <
                        uint8_t,
                        Allocator::DefaultBufferAllocator
                >
        > common_media_packet2;

typedef boost::shared_ptr<common_media_packet2> common_media_packet2_ptr;

} // namespace openmedia

namespace openmedia {

/// \class  common_media_packet2_impl
class _OPENMEDIASDK_API common_media_packet2_impl : public media_packet::Impl
{
public:
    
    class _OPENMEDIASDK_API setter : public media_packet_properties
    {
    public:
        setter(common_media_packet2_impl * _Owner);
        
    private:
        virtual void set_data(boost::shared_array<uint8_t> _Buffer, size_t _Size);
        virtual void set_data(uint8_t * _Buffer, size_t _Size, dt_own_buffer_t _OwnBuffer);
        virtual void set_pts(dt_ts_t _Pts);
        virtual void set_dts(dt_ts_t _Dts);
        virtual void set_stream_index(int _StreamIndex);
        virtual void set_flags(int _Flags);
        virtual void set_duration(dt_media_data_duration_t _Duration);
        virtual void set_byte_pos(dt_filesize_t _BytePos);
        virtual void set_media_type(dt_media_type_t _MediaType);
        virtual void set_time_base(dt_rational_t _TimeBase);
        virtual void set_frame_rate(dt_rational_t frameRate);

    private:
        common_media_packet2_impl * m_owner;

    };

    friend class common_media_packet2_impl::setter;
    template<class> friend class common_media_packet2_t;

public:
    common_media_packet2_impl();
    virtual ~common_media_packet2_impl();

private:
    void final_construct();

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
    boost::shared_array<uint8_t> m_data;
    dt_data_size_t  m_size;
    int             m_stream_index;
    int             m_flags;
    dt_media_data_duration_t m_duration;
    dt_filesize_t   m_byte_pos;
    dt_media_type_t m_media_type;
    bool            m_is_valid;   
    dt_rational_t   m_time_base;
    dt_rational_t   m_frame_rate;

    typedef uint8_t * (*AllocBufferT)(size_t);
    typedef void (*FreeBufferT)(uint8_t **);

    AllocBufferT    m_AllocBuffer;
    FreeBufferT     m_FreeBuffer;
};

inline common_media_packet2_impl::setter::setter(common_media_packet2_impl * _Owner) : m_owner(_Owner)
{    
}

inline void common_media_packet2_impl::setter::set_pts(dt_ts_t _Pts)
{
    m_owner->m_pts = _Pts;
};

inline void common_media_packet2_impl::setter::set_dts(dt_ts_t _Dts)
{
    m_owner->m_dts = _Dts;
};

inline void common_media_packet2_impl::setter::set_stream_index(int _StreamIndex)
{
    m_owner->m_stream_index = _StreamIndex;
};

inline void common_media_packet2_impl::setter::set_flags(int _Flags)
{
    m_owner->m_flags = _Flags;
};

inline void common_media_packet2_impl::setter::set_duration(dt_media_data_duration_t _Duration)
{
    m_owner->m_duration = _Duration;
};

inline void common_media_packet2_impl::setter::set_byte_pos(dt_filesize_t _BytePos)
{
    m_owner->m_byte_pos = _BytePos;
};

inline void common_media_packet2_impl::setter::set_media_type(dt_media_type_t _MediaType)
{
    m_owner->m_media_type = _MediaType;
};

inline void common_media_packet2_impl::setter::set_time_base(dt_rational_t _TimeBase)
{
    m_owner->m_time_base = _TimeBase;
}

inline void common_media_packet2_impl::setter::set_frame_rate(dt_rational_t frameRate)
{
    m_owner->m_frame_rate = frameRate;
}

template <class MemoryAllocator>
void common_media_packet2_t<MemoryAllocator>::final_construct()
{
    common_media_packet2_impl * this_impl = dynamic_cast<common_media_packet2_impl *>( impl() );
    this_impl->m_AllocBuffer = &(this->AllocBuffer);
    this_impl->m_FreeBuffer = &(this->FreeBuffer);
    m_properties = boost::shared_ptr<media_packet_properties>( new common_media_packet2_impl::setter( this_impl ) );
}

template <class MemoryAllocator>
common_media_packet2_t<MemoryAllocator>::common_media_packet2_t() 
: media_packet(new common_media_packet2_impl())
{
    final_construct();
};

/////////////////////////

inline common_media_packet2_impl::~common_media_packet2_impl()
{}

inline dt_ts_t common_media_packet2_impl::get_pts() const
{
    return m_pts;
}

inline dt_ts_t common_media_packet2_impl::get_dts() const
{
    return m_dts;
}

inline const uint8_t * common_media_packet2_impl::get_data() const
{
    return m_data.get();
}

inline dt_data_size_t common_media_packet2_impl::get_size() const 
{
    return m_size;
}

inline int common_media_packet2_impl::get_stream_index() const
{
    return m_stream_index;
}

inline int common_media_packet2_impl::get_flags() const
{
    return m_flags;
}

inline dt_media_data_duration_t common_media_packet2_impl::get_duration() const
{
    return m_duration;
}

inline dt_filesize_t common_media_packet2_impl::get_byte_pos() const
{
    return m_byte_pos;
}

inline dt_media_type_t common_media_packet2_impl::get_media_type() const
{
    return m_media_type;
}

inline dt_rational_t common_media_packet2_impl::get_time_base() const
{
    return m_time_base;
}

inline dt_rational_t common_media_packet2_impl::get_frame_rate() const
{
    return m_frame_rate;
}

inline bool common_media_packet2_impl::is_valid() const
{
    return m_is_valid;
}

} // namespace openmedia

#endif // #ifndef _DTCOMMONPACKET_H_INCLUDED_

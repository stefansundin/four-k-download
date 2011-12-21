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


/// \file   DTCommonCodecExtraData.h

#ifndef _DTCOMMONCODECEXTRADATA_H_INCLUDED_
#define _DTCOMMONCODECEXTRADATA_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <vector>

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/foreach.hpp>
#include <boost/smart_ptr/make_shared.hpp>

#include <openmedia/DTConfig.h>
#include <openmedia/DTTypes.h>
#include <openmedia/DTCStdInt.h>
#include <openmedia/DTCommon.h>
#include <openmedia/DTCodecExtraData.h>

#include "DTAllocator.h"
#include "DTCodecExtraDataImpl.h"

namespace openmedia {

/// \class  class codec_extra_data_setter
class _OPENMEDIASDK_API codec_extra_data_setter : interface_base
{
public:
    virtual void add_data(const std::string & _DataName, uint8_t * _Buffer, size_t _Size, dt_own_buffer_t _OwnBuffer) = 0; 
    virtual void add_data(const std::string & _DataName, boost::shared_array<uint8_t> _Data, size_t _Size) = 0;

};

/// \class  class codec_extra_data_common_t
/// \tparam MemoryAllocator buffer/array allocator 
template <class MemoryAllocator>
class _OPENMEDIASDK_API codec_extra_data_common_t : public codec_extra_data, public MemoryAllocator
{
public:
    codec_extra_data_common_t();
    codec_extra_data_setter * setter();

private:
    boost::shared_ptr<codec_extra_data_setter> m_setter;
    void final_construct();
        
};


/// codec_extra_data_common
typedef codec_extra_data_common_t < 
                Allocator::DefaultArrayAllocator <
                        uint8_t,
                        Allocator::DefaultBufferAllocator
                >
        > codec_extra_data_common;

} // namespace openmedia

namespace openmedia {

/// \class  codec_extra_data_common_impl
class codec_extra_data_common_impl : public codec_extra_data::Impl
{
private:
    class setter : public codec_extra_data_setter
    {
    public:
        setter(codec_extra_data_common_impl * _Owner): m_owner(_Owner)
        {}
        virtual void add_data(const std::string & _DataName, uint8_t * _Buffer, size_t _Size, dt_own_buffer_t _OwnBuffer); 
        virtual void add_data(const std::string & _DataName, boost::shared_array<uint8_t> _Data, size_t _Size);        

    private:
        codec_extra_data_common_impl * m_owner;
    };

    friend class codec_extra_data_common_impl::setter;
    template<class> friend class codec_extra_data_common_t;

public:
    virtual size_t      get_data_count() const;
    virtual uint8_t *   get_data(size_t _DataIndex) const;
    virtual size_t      get_data_size(size_t _DataIndex) const;
    virtual std::string get_data_name(size_t _DataIndex) const;
    virtual bool        get_index_by_name(const std::string & _DataName, size_t & _DataIndex) const;

private:
    typedef std::pair<boost::shared_array<uint8_t>, size_t> BufferAndSizePair;
    typedef std::pair<std::string, BufferAndSizePair> StrAndArrayPair;
    typedef std::vector<StrAndArrayPair> ExtraDataArray;
    
    ExtraDataArray m_ExtraData;
    
    ExtraDataArray & extra_data()
    {
        return const_cast<ExtraDataArray &>
            (static_cast<const codec_extra_data_common_impl *>(this)->extra_data());    
    }

    const ExtraDataArray & extra_data() const
    {
        return m_ExtraData;
    }
  
public:
    codec_extra_data_common_impl(){};
    virtual ~codec_extra_data_common_impl(){};

private:
    typedef uint8_t * (*AllocBufferT)(size_t);
    typedef void (*FreeBufferT)(uint8_t **);

    AllocBufferT    m_AllocBuffer;
    FreeBufferT     m_FreeBuffer;
};


//////////////////////////////////////////////////////////////////////////////////////////

template <class MemoryAllocator>
void codec_extra_data_common_t<MemoryAllocator>::final_construct()
{
    codec_extra_data_common_impl * this_impl = dynamic_cast<codec_extra_data_common_impl *>( impl() );
    this_impl->m_AllocBuffer = &(this->AllocBuffer);
    this_impl->m_FreeBuffer = &(this->FreeBuffer);
    m_setter = boost::make_shared<codec_extra_data_common_impl::setter>(this_impl);
}

template <class MemoryAllocator>
codec_extra_data_common_t<MemoryAllocator>::codec_extra_data_common_t() 
: codec_extra_data(new codec_extra_data_common_impl())
{
    final_construct();
};

template <class MemoryAllocator>
codec_extra_data_setter * codec_extra_data_common_t<MemoryAllocator>::setter()
{
    return m_setter.get();    
}

} // namespace openmedia


#endif // #ifndef _DTCOMMONCODECEXTRADATA_H_INCLUDED_

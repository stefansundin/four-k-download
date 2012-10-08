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

/// \file   DTCommonCodecExtraData.cpp

#include "DTCommonCodecExtraData.h"

#include <vector>

#include <boost/foreach.hpp>

#include <openmedia/DTCodecExtraData.h>
#include "DTCodecExtraDataImpl.h"

#include "DTAllocator.h"
#include "DTCodecExtraDataImpl.h"

namespace openmedia {

void codec_extra_data_common_impl::setter::add_data(const std::string & _DataName, uint8_t * _Buffer, size_t _Size, dt_own_buffer_t _OwnBuffer)
{
    if (bufferRef == _OwnBuffer)
    {
        add_data(_DataName, boost::shared_array<uint8_t>(_Buffer, utils::no_delete()), _Size );
    }
    else if (bufferAllocNew == _OwnBuffer)
    {
        DT_ASSERT(NULL != m_owner->m_AllocBuffer);
        uint8_t * buffer = NULL;
        if (_Size > 0)
        {
            buffer = m_owner->m_AllocBuffer(_Size);
            memcpy(buffer, _Buffer, _Size);
        }
        add_data(_DataName, boost::shared_array<uint8_t> ( buffer, utils::func_deleter<FreeBufferT>( m_owner->m_FreeBuffer ) ), _Size );
    }
    else if (bufferGetOwn == _OwnBuffer)
    {
        add_data(_DataName, boost::shared_array<uint8_t> ( _Buffer, utils::func_deleter<FreeBufferT>( m_owner->m_FreeBuffer ) ), _Size);
    }        
}

void codec_extra_data_common_impl::setter::add_data(const std::string & _DataName, boost::shared_array<uint8_t> _Data, size_t _Size)
{
    m_owner->extra_data().push_back(
    codec_extra_data_common_impl::StrAndArrayPair(
        _DataName,
        codec_extra_data_common_impl::BufferAndSizePair(_Data, _Size)
        )
        );
}

//////////////////////////////////////////////////////////////////////////////////////////

size_t codec_extra_data_common_impl::get_data_count() const 
{
    return extra_data().size();
}

uint8_t * codec_extra_data_common_impl::get_data(size_t _DataIndex) const
{
    return extra_data().at(_DataIndex).second.first.get();
}

size_t codec_extra_data_common_impl::get_data_size(size_t _DataIndex) const
{
    return extra_data().at(_DataIndex).second.second;
}

std::string codec_extra_data_common_impl::get_data_name(size_t _DataIndex) const
{
    return extra_data().at(_DataIndex).first;
}

bool codec_extra_data_common_impl::get_index_by_name(const std::string & _DataName, size_t & _DataIndex) const
{
    const std::vector<StrAndArrayPair> & extraData = extra_data();
    size_t index = 0;
    BOOST_FOREACH(const StrAndArrayPair & elm, extraData)
    {
        if (_DataName == elm.first)
        {
            _DataIndex = index;
            return true;
        }
        ++index;
    }
    return false;
}

}



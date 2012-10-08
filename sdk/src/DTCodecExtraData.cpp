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

/// \file   DTCodecExtraData.cpp

#include <openmedia/DTCodecExtraData.h>

#include <string>

#include "DTCodecExtraDataImpl.h"

namespace openmedia {

size_t codec_extra_data::get_data_count() const
{
    return impl()->get_data_count();
}

uint8_t * codec_extra_data::get_data(size_t _DataIndex) const
{
    return impl()->get_data(_DataIndex);
}

size_t codec_extra_data::get_data_size(size_t _DataIndex) const
{
    return impl()->get_data_size(_DataIndex);
}

std::string codec_extra_data::get_data_name(size_t _DataIndex) const
{
    return impl()->get_data_name(_DataIndex);
}

bool codec_extra_data::get_index_by_name(const std::string & _DataName, size_t & _DataIndex) const
{
    return impl()->get_index_by_name(_DataName, _DataIndex);
}

//////////////////////////////////////////////////////////////////////////////

codec_extra_data::codec_extra_data(codec_extra_data::Impl * _Impl) : m_pImpl(_Impl)
{    
    DT_STRONG_ASSERT(NULL != _Impl);
}

codec_extra_data::~codec_extra_data()
{
    delete m_pImpl;
}

} // namespace openmedia

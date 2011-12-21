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


/// \file   DTCodecExtraDataImpl.h

#ifndef _DTCODECEXTRADATAIMPL_H_INCLUDED_
#define _DTCODECEXTRADATAIMPL_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <openmedia/DTConfig.h>
#include <openmedia/DTCStdInt.h>
#include <openmedia/DTCommon.h>

#include <openmedia/DTCodecExtraData.h>

namespace openmedia {

///  \class codec_extra_data::Impl
class codec_extra_data::Impl
{
public:
    virtual size_t      get_data_count() const = 0;
    virtual uint8_t *   get_data(size_t _DataIndex) const = 0;
    virtual size_t      get_data_size(size_t _DataIndex) const = 0;
    virtual std::string get_data_name(size_t _DataIndex) const = 0;
    virtual bool        get_index_by_name(const std::string & _DataName, size_t & _DataIndex) const = 0;

};

} // namespace openmedia

#endif // #ifndef _DTCODECEXTRADATAIMPL_H_INCLUDED_

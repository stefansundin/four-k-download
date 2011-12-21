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


/// \file   DTCodecExtraData.h

#ifndef _DTCODECEXTRADATA_H_INCLUDED_
#define _DTCODECEXTRADATA_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <openmedia/DTConfig.h>
#include <openmedia/DTDeclareImpl.h>
#include <openmedia/DTCStdInt.h>
#include <openmedia/DTCommon.h>
#include <string>

namespace openmedia {

class codec_extra_data;
typedef boost::shared_ptr<codec_extra_data> codec_extra_data_ptr;

/// \class  codec_extra_data
class _OPENMEDIASDK_API codec_extra_data : interface_base
{
    DT_DECLARE_IMPL2(codec_extra_data);
public:
    size_t      get_data_count() const;
    uint8_t *   get_data(size_t _DataIndex) const;
    size_t      get_data_size(size_t _DataIndex) const;
    std::string get_data_name(size_t _DataIndex) const;
    bool        get_index_by_name(const std::string & _DataName, size_t & _DataIndex) const;
    virtual     ~codec_extra_data();

protected:
    codec_extra_data(codec_extra_data::Impl * _Impl);

private:
    Impl * m_pImpl;

};

} // namespace openmedia

#endif // #ifndef _DTCODECEXTRADATA_H_INCLUDED_

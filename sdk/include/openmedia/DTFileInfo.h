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


/// \file   DTFileInfo.h

#ifndef _DTFILEINFO_H_INCLUDED_
#define _DTFILEINFO_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <openmedia/DTTypes.h>
#include <openmedia/DTDeclareImpl.h>
#include <openmedia/DTConfig.h>
#include <openmedia/DTCommon.h>

namespace openmedia {

class file_info_general;
typedef boost::shared_ptr<file_info_general> file_info_general_ptr;

/// \class  file_info_general
class _OPENMEDIASDK_API file_info_general: interface_base
{
    DT_DECLARE_IMPL2(file_info_general);
public:
    std::wstring    get_file_name() const;
    unsigned int    get_streams_count() const;
    unsigned int    get_chapters_count() const;
    unsigned int    get_programs_count() const;
    std::string     get_format_name() const;
    std::string     get_format_long_name() const;
    dt_ts_t         get_start_time() const;
    dt_ts_t         get_duration() const;
    dt_filesize_t   get_file_size() const;
    int             get_bit_rate() const;
    dt_metadata_t   get_metadata() const;
    dt_rational_t   get_time_base() const;
    virtual         ~file_info_general() = 0;

protected:
    file_info_general(file_info_general::Impl * _Impl);
    void reset(file_info_general::Impl * _Impl);

private:
    file_info_general::Impl * m_pImpl;

};

} // namespace openmedia

#endif // #ifndef _DTFILEINFO_H_INCLUDED_

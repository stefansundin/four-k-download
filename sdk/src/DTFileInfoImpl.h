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


/// \file  DTFileInfoImpl.h

#ifndef _DTFILEINFOIMPL_H_INCLUDED_
#define _DTFILEINFOIMPL_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <openmedia/DTTypes.h>
#include <openmedia/DTCommon.h>

#include <openmedia/DTFileInfo.h>

namespace openmedia {

/// \class  file_info_general::Impl
class file_info_general::Impl: public impl_base
{
public:
    virtual std::string		get_file_name() const = 0;
    virtual unsigned int	get_streams_count() const = 0;
    virtual unsigned int	get_chapters_count() const = 0;
    virtual unsigned int	get_programs_count() const = 0;
    virtual std::string		get_format_name() const = 0;
    virtual std::string		get_format_long_name() const = 0;
    virtual dt_ts_t			get_start_time() const = 0;
    virtual dt_ts_t			get_duration() const = 0;
    virtual dt_filesize_t	get_file_size() const = 0;
    virtual int				get_bit_rate() const = 0;
    virtual dt_metadata_t	get_metadata() const = 0;
    virtual dt_rational_t   get_time_base() const = 0;

};

} // namespace openmedia

#endif

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


/// \file   DTFFFileInfo.h

#ifndef _DTFFFILEINFOIMPL_H_INCLUDED_
#define _DTFFFILEINFOIMPL_H_INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <openmedia/DTCommon.h>
#include "../DTFileInfoImpl.h"

struct AVFormatContext;

namespace openmedia {

/// \class  file_info_general
class ff_file_info_general_impl : public file_info_general::Impl
{
public:
    virtual std::string		get_file_name() const;
    virtual unsigned int	get_streams_count() const;
    virtual unsigned int	get_chapters_count() const;
    virtual unsigned int	get_programs_count() const;
    virtual std::string		get_format_name() const;
    virtual std::string		get_format_long_name() const;
    virtual dt_ts_t			get_start_time() const;
    virtual dt_ts_t			get_duration() const;
    virtual dt_filesize_t	get_file_size() const;
    virtual int				get_bit_rate() const;
    virtual dt_metadata_t	get_metadata() const;
    virtual dt_rational_t   get_time_base() const;
    virtual                 ~ff_file_info_general_impl();

public:
    ff_file_info_general_impl(AVFormatContext * _Formatcontext);
    void set(AVFormatContext * _Formatcontext);

private:
	AVFormatContext * m_FormatContext;
}; 

} // namespace openmedia

#endif // #ifndef _DTFFFILEINFOIMPL_H_INCLUDED_

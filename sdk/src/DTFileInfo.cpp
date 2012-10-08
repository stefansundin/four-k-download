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

#include <cstdio>
#include <string>

#include <openmedia/DTCommon.h>
#include <openmedia/DTFileInfo.h>
#include <openmedia/DTString.h>
#include "DTFileInfoImpl.h"

namespace openmedia {

std::wstring file_info_general::get_file_name() const
{
  //  return impl()->get_file_name();
    return utf8_to_utf16(impl()->get_file_name());
}

unsigned int file_info_general::get_streams_count() const
{
    return impl()->get_streams_count();
}

unsigned int file_info_general::get_chapters_count() const
{
    return impl()->get_chapters_count();
}

unsigned int file_info_general::get_programs_count() const
{
    return impl()->get_programs_count();
}

std::string file_info_general::get_format_name() const
{
    return impl()->get_format_name();
}

std::string file_info_general::get_format_long_name() const
{
    return impl()->get_format_long_name();
}

dt_ts_t file_info_general::get_start_time() const
{
    return impl()->get_start_time();
}

dt_ts_t file_info_general::get_duration() const
{
    return impl()->get_duration();
}

dt_filesize_t file_info_general::get_file_size() const
{
    return impl()->get_file_size();
}

int file_info_general::get_bit_rate() const
{
    return impl()->get_bit_rate();
}

dt_metadata_t file_info_general::get_metadata() const
{
    return impl()->get_metadata();
}

dt_rational_t   file_info_general::get_time_base() const
{
    return impl()->get_time_base();
}
 
file_info_general::~file_info_general()
{
    delete m_pImpl;
}
   
file_info_general::file_info_general(file_info_general::Impl * _Impl): m_pImpl(_Impl)
{
    DT_STRONG_ASSERT(NULL != _Impl);
}

void file_info_general::reset(file_info_general::Impl * _Impl)
{
    DT_STRONG_ASSERT(NULL != _Impl);
    delete m_pImpl;
    m_pImpl = _Impl;
}

} // namespace openmedia

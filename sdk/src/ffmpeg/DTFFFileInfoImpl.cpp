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

#include "DTFFFileInfoImpl.h"

#define DT_FF_NEED_AVFORMAT
#include "DTFFHeader.h"


namespace openmedia {

ff_file_info_general_impl::ff_file_info_general_impl(AVFormatContext * _FormatContext)
{
    set(_FormatContext);
}

void ff_file_info_general_impl::set(AVFormatContext * _FormatContext)
{
    DT_ASSERT(NULL != _FormatContext);
    if (NULL == m_FormatContext)
    {
        // TODO: throw exception
    }
    m_FormatContext = _FormatContext;
}

std::string ff_file_info_general_impl::get_file_name() const
{
    DT_ASSERT(NULL != m_FormatContext);
    return std::string(m_FormatContext->filename);
}

unsigned int ff_file_info_general_impl::get_streams_count() const
{
    DT_ASSERT(NULL != m_FormatContext);
    return m_FormatContext->nb_streams;	
}

unsigned int ff_file_info_general_impl::get_chapters_count() const
{
    DT_ASSERT(NULL != m_FormatContext);
    return m_FormatContext->nb_chapters;	
}

unsigned int ff_file_info_general_impl::get_programs_count() const
{
    DT_ASSERT(NULL != m_FormatContext);
    return m_FormatContext->nb_programs;	
}

std::string ff_file_info_general_impl::get_format_name() const
{
    DT_ASSERT(NULL != m_FormatContext);
    DT_ASSERT(NULL != m_FormatContext->iformat);

    if (m_FormatContext->iformat)
        return std::string(m_FormatContext->iformat->name);
    else
        return std::string("");
}

std::string ff_file_info_general_impl::get_format_long_name() const
{
    DT_ASSERT(NULL != m_FormatContext);
    DT_ASSERT(NULL != m_FormatContext->iformat);

    if (m_FormatContext->iformat)
        return std::string(m_FormatContext->iformat->long_name);
    else
        return std::string("");
}

dt_ts_t ff_file_info_general_impl::get_start_time() const
{
    DT_ASSERT(NULL != m_FormatContext);

    return m_FormatContext->start_time;	
}

dt_ts_t ff_file_info_general_impl::get_duration() const
{
    DT_ASSERT(NULL != m_FormatContext);

    return m_FormatContext->duration;
}

dt_filesize_t ff_file_info_general_impl::get_file_size() const
{
    //DT_ASSERT(NULL != m_FormatContext);
    DT_ASSERT(false);
    // TODO remove it
    return 0;
}

int ff_file_info_general_impl::get_bit_rate() const
{
    DT_ASSERT(NULL != m_FormatContext);
    return m_FormatContext->bit_rate;
}

dt_metadata_t ff_file_info_general_impl::get_metadata() const
{
    DT_ASSERT(NULL != m_FormatContext);
    return dt_metadata_t();
}

dt_rational_t ff_file_info_general_impl::get_time_base() const
{
    return dt_rational_t(1, AV_TIME_BASE);
}

ff_file_info_general_impl::~ff_file_info_general_impl()
{

}

} // namespace openmedia

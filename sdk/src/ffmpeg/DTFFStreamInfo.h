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


/// \file  DTFFStreamInfo.h

#ifndef _DTFFSTREAMINFO_H_INCLUDED_
#define _DTFFSTREAMINFO_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <openmedia/DTCommon.h>
#include <openmedia/DTStreamInfo.h>
#include "../DTStreamInfoImpl.h"

struct AVStream;
struct AVFormatContext;

namespace openmedia {

/// \class  ff_stream_info_impl
class ff_stream_info_impl: public stream_info::Impl
{
private:
    virtual int                 get_index_impl() const
    {
        return m_index;
    }

    virtual int                 get_id_impl() const
    {
        return m_id;
    }

    virtual dt_rational_t       get_frame_rate_impl() const
    {
        return m_frame_rate;
    }

    virtual dt_ts_t             get_first_dts_impl() const
    {
        return m_first_dts;
    }

    virtual dt_rational_t       get_time_base_impl() const
    {
        return m_time_base;
    }

    virtual dt_ts_t             get_start_time_impl() const
    {
        return m_start_time;
    }

    virtual dt_duration_t       get_duration_impl() const
    {
        return m_duration;
    }

    virtual std::string         get_language_impl() const
    {
        return m_language;
    }

    virtual dt_count_t          get_frames_count_impl() const
    {
        return m_frames_count;
    }

    virtual dt_rational_t       get_sample_aspect_ratio_impl() const
    {
        return m_sample_aspect_ratio;
    }

    virtual metadata_ptr        get_metadata_impl() const
    {
        return m_metadata;
    }

    virtual dt_rational_t       get_avg_frame_rate_impl() const
    {
        return m_avg_frame_rate;
    }

    virtual decoder_info_ptr    get_decoder_info_impl() const
    {
        return m_decoder_info;
    }

public:
    ff_stream_info_impl(const AVFormatContext * avFormatContext, const AVStream * _AVStream);

private:
    int                 m_index;
    int                 m_id;
    dt_rational_t       m_frame_rate;
    dt_ts_t             m_first_dts;
    dt_rational_t       m_time_base;
    dt_ts_t             m_start_time;
    dt_duration_t       m_duration;
    std::string         m_language;
    dt_count_t          m_frames_count;
    dt_rational_t       m_sample_aspect_ratio;
    metadata_ptr        m_metadata;
    dt_rational_t       m_avg_frame_rate;
    decoder_info_ptr    m_decoder_info;

};

/// \class  ff_stream_info
class ff_stream_info: public stream_info
{
public:
    ff_stream_info(const AVFormatContext * avFormatContext, const AVStream * _AVStream): stream_info( new ff_stream_info_impl(avFormatContext, _AVStream) )
    {
    }
    
    static stream_info_ptr create(const AVFormatContext * avFormatContext, const AVStream * _AVStream)
    {
        return stream_info_ptr(new ff_stream_info(avFormatContext, _AVStream));
    }

};

} // namespace openmedia

#endif // #ifndef _DTFFSTREAMINFO_H_INCLUDED_

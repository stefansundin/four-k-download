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

/// \file   DTStreamInfo.cpp

#include <cstdio>
#include <string>

#include <openmedia/DTStreamInfo.h>
#include "DTStreamInfoImpl.h"

namespace openmedia {

stream_info::~stream_info()
{
    delete m_pImpl;
}

stream_info::stream_info(stream_info::Impl * _Impl):m_pImpl(_Impl)
{
    DT_STRONG_ASSERT(NULL != _Impl);
}

int stream_info::get_index() const
{
    return impl()->get_index();
}

int stream_info::get_id() const
{
    return impl()->get_id();
}

dt_rational_t stream_info::get_frame_rate() const
{
    return impl()->get_frame_rate();
}

dt_ts_t stream_info::get_first_dts() const
{
    return impl()->get_first_dts();
}

dt_rational_t stream_info::get_time_base() const
{
    return impl()->get_time_base();
}

dt_ts_t stream_info::get_start_time() const
{
    return impl()->get_start_time();
}

dt_duration_t stream_info::get_duration() const
{
    return impl()->get_duration();
}

std::string stream_info::get_language() const
{
    return impl()->get_language();
}

dt_count_t stream_info::get_frames_count() const
{
    return impl()->get_frames_count();
}

dt_rational_t stream_info::get_sample_aspect_ratio() const
{
    return impl()->get_sample_aspect_ratio();
}

metadata_ptr stream_info::get_metadata() const
{
    return impl()->get_metadata();
}

dt_rational_t stream_info::get_avg_frame_rate() const
{
    return impl()->get_avg_frame_rate();
}

decoder_info_ptr stream_info::get_decoder_info() const
{
    return impl()->get_decoder_info();
}

} // namespace openmedia


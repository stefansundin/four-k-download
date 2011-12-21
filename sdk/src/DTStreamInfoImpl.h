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


/// \file   DTStreamInfoImpl.h

#ifndef _DTSTREAMINFOIMPL_H_INCLUDED_
#define _DTSTREAMINFOIMPL_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <string>

#include <openmedia/DTTypes.h>
#include <openmedia/DTCommon.h>
#include <openmedia/DTDecoderInfo.h>

namespace openmedia {

/// \class  stream_info::Impl
class stream_info::Impl: public impl_base
{
public:
    int                 get_index() const;
    int                 get_id() const;
    dt_rational_t       get_frame_rate() const;
    dt_ts_t             get_first_dts() const;
    dt_rational_t       get_time_base() const;
    dt_ts_t             get_start_time() const;
    dt_duration_t       get_duration() const;
    std::string         get_language() const;
    dt_count_t          get_frames_count() const;
    dt_rational_t       get_sample_aspect_ratio() const;
    metadata_ptr        get_metadata() const;
    dt_rational_t       get_avg_frame_rate() const;
    decoder_info_ptr    get_decoder_info() const;        

private:
    virtual int                 get_index_impl() const = 0;
    virtual int                 get_id_impl() const = 0;
    virtual dt_rational_t       get_frame_rate_impl() const = 0;
    virtual dt_ts_t             get_first_dts_impl() const = 0;
    virtual dt_rational_t       get_time_base_impl() const = 0;
    virtual dt_ts_t             get_start_time_impl() const = 0;
    virtual dt_duration_t       get_duration_impl() const = 0;
    virtual std::string         get_language_impl() const = 0;
    virtual dt_count_t          get_frames_count_impl() const = 0;
    virtual dt_rational_t       get_sample_aspect_ratio_impl() const = 0;
    virtual metadata_ptr        get_metadata_impl() const = 0;
    virtual dt_rational_t       get_avg_frame_rate_impl() const = 0;
    virtual decoder_info_ptr    get_decoder_info_impl() const = 0;       
};

inline int stream_info::Impl::get_index() const
{
    return get_index_impl();
};

inline int stream_info::Impl::get_id() const
{
    return get_id_impl();
};

inline dt_rational_t stream_info::Impl::get_frame_rate() const
{
    return get_frame_rate_impl();
};

inline dt_ts_t stream_info::Impl::get_first_dts() const
{
    return get_first_dts_impl();
};

inline dt_rational_t stream_info::Impl::get_time_base() const
{
    return get_time_base_impl();
};

inline dt_ts_t stream_info::Impl::get_start_time() const
{
    return get_start_time_impl();
};

inline dt_duration_t stream_info::Impl::get_duration() const
{
    return get_duration_impl();
};

inline std::string stream_info::Impl::get_language() const
{
    return get_language_impl();
};

inline dt_count_t stream_info::Impl::get_frames_count() const
{
    return get_frames_count_impl();
};

inline dt_rational_t stream_info::Impl::get_sample_aspect_ratio() const
{
    return get_sample_aspect_ratio_impl();
};

inline metadata_ptr stream_info::Impl::get_metadata() const
{
    return get_metadata_impl();
};

inline dt_rational_t stream_info::Impl::get_avg_frame_rate() const
{
    return get_avg_frame_rate_impl();
};

inline decoder_info_ptr stream_info::Impl::get_decoder_info() const
{
    return get_decoder_info_impl();
}

} // namespace openmedia
#endif

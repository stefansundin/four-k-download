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


/// \file   DTStreamInfo.h

#ifndef _DTSTREAMINFO_H_INCLUDED_
#define _DTSTREAMINFO_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <openmedia/DTDeclareImpl.h>
#include <openmedia/DTCommon.h>

#include <openmedia/DTDecoderInfo.h>
#include <openmedia/DTMetadata.h>

namespace openmedia {

class stream_info;
typedef boost::shared_ptr<stream_info> stream_info_ptr;

/// \class  stream_info
class _OPENMEDIASDK_API stream_info: public interface_base
{
    DT_DECLARE_IMPL2(stream_info);
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

    virtual ~stream_info() = 0;

protected:
    stream_info(stream_info::Impl * _Impl);

private:
    stream_info::Impl * m_pImpl;

};

} // namespace openmedia

#endif

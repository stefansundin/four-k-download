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

/// \file   DTFFStreamInfo.cpp

#include <openmedia/DTCommon.h>
#include <openmedia/DTError.h>
#include "DTFFStreamInfo.h"
//#include "DTFFMetadata.h"
#include "DTFFDecoderInfo.h"

#define DT_FF_NEED_AVFORMAT
#include "DTFFHeader.h"

namespace openmedia {

namespace {

int64_t convertDuration(int64_t duration, AVRational oldBase, AVRational newBase)
{
    return static_cast<int64_t>(1.0 * duration / oldBase.den * oldBase.num * newBase.den / newBase.num + 0.5);
}

}

ff_stream_info_impl::ff_stream_info_impl(const AVFormatContext * avFormatContext, const AVStream * _AVStream)
{
    DT_ASSERT(NULL != _AVStream);
    if (NULL == _AVStream)
        BOOST_THROW_EXCEPTION(errors::invalid_pointer());

    m_index = _AVStream->index;
    m_id = _AVStream->id;
    m_frame_rate = FF2DTType(_AVStream->r_frame_rate);
    m_first_dts = _AVStream->first_dts;
    m_time_base = FF2DTType(_AVStream->time_base);
    m_start_time = _AVStream->start_time;

    const AVRational tb = {1, AV_TIME_BASE};
    m_duration = (_AVStream->duration != AV_NOPTS_VALUE) ? _AVStream->duration : convertDuration(avFormatContext->duration,  tb, _AVStream->time_base);;

    m_language = /*_AVStream->language*/""; // remove from ffmpeg 0.8
    m_frames_count = _AVStream->nb_frames;
    m_sample_aspect_ratio = FF2DTType(_AVStream->sample_aspect_ratio);
    m_metadata = metadata_ptr(/*new ff_metadata(_AVStream->metadata)*/);
    m_avg_frame_rate = FF2DTType(_AVStream->avg_frame_rate);
    m_decoder_info = create_ff_decoder_info(_AVStream->codec);
}

} // namespace openmedia 

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

/// \file   DTVideoEncoderQueue.cpp

#include <openmedia/DTVideoEncoderQueue.h>
#include "DTVideoEncoderQueueImpl.h"

namespace openmedia {

video_encoder_queue::video_encoder_queue(video_encoder_queue::Impl * _Impl) : m_pImpl(_Impl)
{
    DT_STRONG_ASSERT(!!_Impl);
}

void video_encoder_queue::open()
{
    return impl()->open();
}

void video_encoder_queue::send_video(const video_data * _VideoData)
{
    return impl()->send_video(_VideoData);
}

void video_encoder_queue::send_video(const video_data_timed * _VideoDataTimed)
{
    return impl()->send_video(_VideoDataTimed);
}

media_packet_ptr video_encoder_queue::receive_packet()
{
    return impl()->receive_packet();
}

void * video_encoder_queue::get_private_data() const
{
    return impl()->get_private_data();
}

} // namespace openmedia

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


/// \file   DTVideoEncoderQueueImpl.h

#ifndef _DTVIDEOENCODERQUEUEIMPL_H_INCLUDED_
#define _DTVIDEOENCODERQUEUEIMPL_H_INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <openmedia/DTCommon.h>
#include <openmedia/DTVideoEncoderQueue.h>

namespace openmedia {

class media_packet;
typedef boost::shared_ptr<media_packet> media_packet_ptr;

class video_data;

class video_encoder_queue::Impl : public impl_base
{
public:
    virtual void                open() = 0;
    virtual void                send_video(const video_data * _VideoData) = 0;
    virtual void                send_video(const video_data_timed * _VideoDataTimed);
    virtual media_packet_ptr    receive_packet() = 0;
    virtual void *              get_private_data() const;
};

inline void * video_encoder_queue::Impl::get_private_data() const
{
    return NULL;
}

} // namespace openmedia

#endif

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


/// \file   DTAudioEncoderQueueImpl.h

#ifndef _DTAUDIOENCODERQUEUEIMPL_H_INCLUDED_
#define _DTAUDIOENCODERQUEUEIMPL_H_INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <openmedia/DTCommon.h>
#include <openmedia/DTAudioEncoder.h>
#include <openmedia/DTPacket.h>
#include <openmedia/DTAudioData.h>

namespace openmedia {

/// \class  audio_encoder_queue::Impl
class audio_encoder_queue::Impl : impl_base
{
public:
    virtual void                    open() = 0;
    virtual void                    send_audio(const audio_data * _AudioData) = 0;  
    virtual void                    send_audio(const audio_data_timed * audioDataTimed) = 0;
    virtual media_packet_ptr        receive_packet() = 0;
    virtual codec_extra_data_ptr    get_extra_data() const = 0;
    virtual void *                  get_private_data() const;
};

inline void * audio_encoder_queue::Impl::get_private_data() const
{
    return NULL;
}

} // namespace openmedia

#endif // #ifndef _DTAUDIOENCODERQUEUEIMPL_H_INCLUDED_

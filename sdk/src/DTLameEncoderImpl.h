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


/// \file   DTLameEncoderImpl.h

#ifndef _DTLAMEENCODERIMPL_H_INCLUDED_
#define _DTLAMEENCODERIMPL_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "DTLameEncoder.h"
#include "DTAudioEncoderImpl.h"
#include "DTAudioEncoderQueueImpl.h"
#include "DTCommonCodecExtraData.h"

#include <openmedia/DTAudioEncoder.h>
#include <openmedia/DTAudioData.h>
#include <openmedia/DTAudioDataTimed.h>
#include <openmedia/DTAudioFormat.h>
#include <openmedia/DTAudioBuffer.h>

namespace openmedia {

class lame_encoder_impl_internal;

class audio_encoder_queue_lame_impl : public audio_encoder_queue::Impl
{
public:
    audio_encoder_queue_lame_impl(const audio_format * _AudioFormat, const audio_encoder_lame_utils::properties * _Properties);
    virtual ~audio_encoder_queue_lame_impl(); 

public:
    virtual void                    open(){};
    virtual void                    send_audio(const audio_data * _AudioData);
    virtual void                    send_audio(const audio_data_timed * audioDataTimed);    
    virtual media_packet_ptr        receive_packet();
    virtual codec_extra_data_ptr    get_extra_data() const;

private:
    lame_encoder_impl_internal * m_Lame;
    boost::shared_ptr<codec_extra_data_common> m_CodecExtraData;
    audio_buffer_ptr m_AudioBuffer;
    size_t sampleRate_;
    bool lastData_;
};

}

#endif

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

/// \file   DTAudioEncoder.cpp

#include <openmedia/DTAudioEncoder.h>

#include "DTAudioEncoderQueueImpl.h"

#include "DTLameEncoder.h"
#include "DTAudioEncoderSettingsImpl.h"

namespace openmedia {

audio_encoder_queue::~audio_encoder_queue()
{
    delete m_pImpl;
}

void audio_encoder_queue::open()
{
    return impl()->open();
}

void audio_encoder_queue::send_audio(const audio_data * audioData)
{
    return impl()->send_audio(audioData);
}

void audio_encoder_queue::send_audio(const audio_data_timed * audioDataTimed)
{
    return impl()->send_audio(audioDataTimed);
}

void audio_encoder_queue::flush()
{
    return impl()->send_audio(static_cast<audio_data*>(NULL));
}

media_packet_ptr audio_encoder_queue::receive_packet()
{
    return impl()->receive_packet();
}

audio_encoder_queue::audio_encoder_queue(audio_encoder_queue::Impl * _Impl) : m_pImpl( _Impl )
{
    DT_STRONG_ASSERT(NULL != _Impl);    
}

codec_extra_data_ptr audio_encoder_queue::get_extra_data() const
{
    return impl()->get_extra_data();
}

void * audio_encoder_queue::get_private_data() const
{
    return impl()->get_private_data();
}

audio_encoder_queue_ptr audio_encoder_creator::create_lame_mp3_encoder_queue(const audio_format * _InputAudioFormat, const char * Artist, const char * Title, int Bitrate)
{
    audio_encoder_lame_utils::properties_ptr prop = audio_encoder_lame_utils::create_properties(Artist, Title, Bitrate);
    return audio_encoder_queue_ptr( new audio_encoder_queue_lame(_InputAudioFormat, prop.get()) );
}

audio_encoder_queue_ptr audio_encoder_creator::create_lame_mp3_encoder_queue(const audio_format * _InputAudioFormat)
{
    audio_encoder_lame_utils::properties_ptr prop = audio_encoder_lame_utils::create_properties();
    return audio_encoder_queue_ptr( new audio_encoder_queue_lame(_InputAudioFormat, prop.get()) );
}

}

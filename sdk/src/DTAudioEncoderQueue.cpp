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




#include <openmedia/DTHeaders.h>


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

void audio_encoder_queue::send_audio(const audio_data * _AudioData)
{
    return impl()->send_audio(_AudioData);
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

audio_encoder_queue_ptr audio_encoder_utils::create_lame_mp3_encoder_queue(const audio_format * _InputAudioFormat, const char * Artist, const char * Title)
{
    audio_encoder_lame_utils::properties_ptr prop = audio_encoder_lame_utils::create_properties(Artist, Title);
    return audio_encoder_queue_ptr( new audio_encoder_queue_lame(_InputAudioFormat, prop.get()) );
}

audio_encoder_queue_ptr audio_encoder_utils::create_lame_mp3_encoder_queue(const audio_format * _InputAudioFormat)
{
    audio_encoder_lame_utils::properties_ptr prop = audio_encoder_lame_utils::create_properties();
    return audio_encoder_queue_ptr( new audio_encoder_queue_lame(_InputAudioFormat, prop.get()) );
}

}

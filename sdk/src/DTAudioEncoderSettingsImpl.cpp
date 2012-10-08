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
#include "DTAudioEncoderSettingsImpl.h"

namespace openmedia {

EncoderMode audio_encoder_settings::get_mode() const { return impl()->get_mode(); }
int audio_encoder_settings::get_bit_rate() const { return impl()->get_bit_rate(); }
double audio_encoder_settings::get_quality() const { return impl()->get_quality(); }

audio_encoder_settings::audio_encoder_settings(Impl * _Impl) : m_pImpl(_Impl)
{
    DT_STRONG_ASSERT(NULL != _Impl);
}

audio_encoder_settings::~audio_encoder_settings()
{
    delete m_pImpl;
}

audio_encoder_settings_ptr audio_encoder_settings::create_bitrate_base(int BitRate)
{
    return audio_encoder_settings_ptr( new audio_encoder_settings( new audio_encoder_settings::Impl(encoderBitrateBase, BitRate, 0.0) ) );    
}

audio_encoder_settings_ptr audio_encoder_settings::create_quality_base(double Quality)
{
    return audio_encoder_settings_ptr( new audio_encoder_settings( new audio_encoder_settings::Impl(encoderQualityBase, 0, Quality) ) );    
}

audio_encoder_settings_ptr audio_encoder_settings::duplicate() const
{
    return audio_encoder_settings_ptr( new audio_encoder_settings(
        new audio_encoder_settings::Impl( get_mode(), get_bit_rate(), get_quality() )
        ) );
}

audio_encoder_settings_ptr audio_encoder_settings::clone() const
{
    return this->duplicate();
}

//////////////////////////////////////////////////////////////////////////

} // namespace openmedia

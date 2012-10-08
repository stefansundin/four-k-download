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

/// \file   DTAudioData.cpp

#include <openmedia/DTAudioData.h>
#include "DTAudioDataImpl.h"

namespace openmedia {

audio_data::audio_data():m_pImpl(NULL) //? TODO
{
}

void audio_data::reset_impl(audio_data::Impl * _Impl)
{
	DT_STRONG_ASSERT(NULL != _Impl);
	delete m_pImpl;
	m_pImpl = _Impl;
}

audio_data::audio_data(audio_data::Impl * _Impl)
{
    DT_ASSERT(NULL != _Impl);
    m_pImpl = _Impl;
}

audio_data::~audio_data()
{
    delete m_pImpl;
}

const uint8_t * audio_data::get_raw_data() const
{
    return impl()->get_raw_data();
}

dt_data_size_t audio_data::get_raw_size() const
{
    return impl()->get_raw_size();
}

ptrdiff_t audio_data::get_sample_align() const
{
    return impl()->get_sample_align();
}

int audio_data::get_samples_count() const
{
    return impl()->get_samples_count();
}

dt_sample_format_t audio_data::get_sample_format() const
{
    return impl()->get_sample_format();
}

dt_channel_layout_t audio_data::get_channel_layout() const
{
    return impl()->get_channel_layout();
}

int audio_data::get_channels_count() const
{
    return impl()->get_channels_count();
}

int audio_data::get_bits_per_sample() const
{
    return impl()->get_bits_per_sample();
}

const uint8_t * audio_data::get_data_at(int _ChannelNum, int _SampleNum) const
{
    return impl()->get_data_at(_ChannelNum, _SampleNum);
}

uint8_t * audio_data::lock_data()
{
    return impl()->lock_data();
}

void audio_data::unlock_data()
{
    return impl()->unlock_data();
}

} // namespace openmedia 

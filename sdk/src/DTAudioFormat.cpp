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


/// \file   DTAudioFormat.h

#include <openmedia/DTAudioFormat.h>
#include <openmedia/DTAssert.h>
#include "DTAudioFormatImpl.h"

namespace openmedia {

dt_sample_format_t audio_format::get_sample_format() const
{
    return impl()->get_sample_format();
}

dt_channel_layout_t audio_format::get_channel_layout() const
{
    return impl()->get_channel_layout();
}

int audio_format::get_channels_count() const
{
    return impl()->get_channels_count();
}

int audio_format::get_bits_per_sample() const
{
    return impl()->get_bits_per_sample();
}

ptrdiff_t audio_format::get_sample_align() const
{
    return impl()->get_sample_align();
}

int audio_format::get_sample_rate() const
{
    return impl()->get_sample_rate();
}

audio_format::audio_format(audio_format::Impl * _Impl):m_pImpl(_Impl)
{
    DT_STRONG_ASSERT(NULL != _Impl);
}

audio_format::~audio_format()
{
    delete m_pImpl;
}

audio_format_ptr audio_format::duplicate() const
{
    return create( this );
}

audio_format_ptr audio_format::clone() const
{
    return this->duplicate();
}

audio_format_ptr audio_format::create(int _SampleRate, dt_sample_format_t _SampleFormat, int _ChannelsCount)
{
    return create(_SampleRate, _SampleFormat, _ChannelsCount, 0); // TODO!
}

audio_format_ptr audio_format::create(int _SampleRate, dt_sample_format_t _SampleFormat, int _ChannelsCount, dt_channel_layout_t _ChannelLayout)
{
    return audio_format_ptr( new audio_format( new audio_format::Impl(_SampleRate, _SampleFormat, _ChannelsCount, _ChannelLayout) ) );
}

audio_format_ptr audio_format::create(const audio_format * _AudioFormat)
{
    return create( _AudioFormat->get_sample_rate(), _AudioFormat->get_sample_format(), _AudioFormat->get_channels_count(), _AudioFormat->get_channel_layout() );
}

size_t audio_format::size_bytes(const audio_format * _AudioFormat, size_t _SamplesCount)
{
    DT_ASSERT(NULL != _AudioFormat);
    return _SamplesCount * _AudioFormat->get_bits_per_sample() / (std::numeric_limits<uint8_t>::digits) * _AudioFormat->get_channels_count();
}

audio_format::invalid_format::invalid_format()
{
    m_msg = "invalid audio format";
}


}

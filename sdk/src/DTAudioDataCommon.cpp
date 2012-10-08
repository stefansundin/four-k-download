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

/// \file   DTAudioDataCommon.cpp

#include <openmedia/DTAudioDataCommon.h>

#include <cstring>

#include "DTAudioDataImpl.h"
#include <openmedia/DTAudioUtils.h>
#include <openmedia/DTAudioDecoderInfo.h>
#include <openmedia/DTAudioUtils.h>
#include <openmedia/DTAudioFormat.h>

namespace openmedia {

uint8_t * audio_data_common::alloc_buffer(size_t _SizeBytes)
{
    return new uint8_t[_SizeBytes];
}

void audio_data_common::free_buffer(uint8_t * _Buffer)
{
    delete [] _Buffer;
}

/// \class  audio_data_common_impl
class audio_data_common_impl: public audio_data::Impl
{
public:
    audio_data_common_impl();
    void set_properties(int _SampleRate, int _ChannelsCount, dt_sample_format_t _SampleFormat, int _BlockAlign, dt_channel_layout_t _ChannelsLayout);
    void resize(size_t _SizeBytes);
    void set_buffer(uint8_t * _Buffer, size_t _SizeBytes, audio_data_common::OwnBuffer _OwnBuffer);
    virtual ~audio_data_common_impl();

public:
    virtual const uint8_t *     get_raw_data() const;
    virtual dt_data_size_t      get_raw_size() const;
    virtual ptrdiff_t           get_sample_align() const;

    virtual int                 get_samples_count() const;
    virtual dt_sample_format_t  get_sample_format() const;
    virtual dt_channel_layout_t get_channel_layout() const;
    virtual int                 get_channels_count() const;
    virtual int                 get_bits_per_sample() const;
    virtual uint8_t *           lock_data();
    virtual void                unlock_data();

private:
    
    uint8_t *           m_Buffer;
    size_t              m_BufferSize;
    int                 m_DataSize;
    bool                m_OwnBuffer;

    int                 m_SamplesCount;

    dt_sample_format_t  m_SampleFormat;
    dt_channel_layout_t m_ChannelLayout;
    int                 m_ChannelsCount;
    int                 m_BitsPerSample;
    int                 m_SampleRate;
    ptrdiff_t           m_SampleAlign;

};

/// audio_data_common_impl implementation 

audio_data_common_impl::audio_data_common_impl(): m_Buffer(NULL),
m_BufferSize(0),
m_DataSize(0),
m_OwnBuffer(false),
m_SamplesCount(0),
m_SampleFormat(DT_SAMPLE_FMT_NONE),
m_ChannelLayout(0),
m_ChannelsCount(0),
m_BitsPerSample(0),
m_SampleRate(0),
m_SampleAlign(0)
{
}


audio_data_common::audio_data_common(): audio_data(new audio_data_common_impl())
{
}

audio_data_common::audio_data_common(const audio_decoder_info * _AudioDecoderInfo): audio_data(new audio_data_common_impl())
{
    audio_data_common_impl * cimpl = static_cast<audio_data_common_impl*>(impl());
    cimpl->set_properties(_AudioDecoderInfo->get_sample_rate(),
        _AudioDecoderInfo->get_channels_count(),
        _AudioDecoderInfo->get_sample_format(),
        _AudioDecoderInfo->get_sample_align(),
        _AudioDecoderInfo->get_channel_layout()
        );
}

audio_data_common_impl::~audio_data_common_impl()
{
    if (m_OwnBuffer)
    {
        audio_data_common::free_buffer(m_Buffer);
    }
}


void audio_data_common_impl::set_buffer(uint8_t * _Buffer, size_t _SizeBytes, audio_data_common::OwnBuffer _OwnBuffer)
{
    if (audio_data_common::bufferAllocNew == _OwnBuffer)
    {
        uint8_t * newBuffer = audio_data_common::alloc_buffer(_SizeBytes);
        
        if (_Buffer)
        {
            memcpy(newBuffer, _Buffer, _SizeBytes);
        }

        if (m_OwnBuffer)
        {
            audio_data_common::free_buffer(m_Buffer);
        }
        
        m_Buffer        = newBuffer;
        m_BufferSize    = _SizeBytes;
        m_DataSize      = _SizeBytes;
        m_OwnBuffer     = true;

    }
    else if (audio_data_common::bufferGetOwn == _OwnBuffer || audio_data_common::bufferRef == _OwnBuffer)
    {
        if (m_OwnBuffer)
        {
            audio_data_common::free_buffer(m_Buffer);
        }
        
        m_Buffer = _Buffer;        
        m_BufferSize = _SizeBytes;
        m_DataSize   = _SizeBytes;
        m_OwnBuffer = (audio_data_common::bufferGetOwn == _OwnBuffer);
    }
    else
    {
        DT_ASSERT(false);
    }

    DT_ASSERT(0 != m_SampleAlign);
    m_SamplesCount = m_BufferSize / m_SampleAlign;
}

void audio_data_common_impl::set_properties(int _SampleRate, int _ChannelsCount, dt_sample_format_t _SampleFormat, int _BlockAlign, dt_channel_layout_t _ChannelLayout)
{
    DT_ASSERT(_SampleRate == 48000 ||
        _SampleRate == 44100 ||
        _SampleRate == 22050 ||
        _SampleRate == 11025 ||
        _SampleRate == 8000 ||
        _SampleRate == 16000 ||
        _SampleRate == 32000 || 
        _SampleRate == 96000
        );

    DT_ASSERT(_ChannelsCount > 0);
    DT_ASSERT(_BlockAlign == utils::audio::bits_to_bytes(
        utils::audio::sample_fmt_to_bps(_SampleFormat)
        ) * _ChannelsCount);

    m_SampleRate    = _SampleRate;
    m_ChannelsCount = _ChannelsCount;
    m_SampleFormat  = _SampleFormat;
    m_BitsPerSample = utils::audio::sample_fmt_to_bps(_SampleFormat);
    m_SampleAlign   = _BlockAlign;
	m_ChannelLayout	= _ChannelLayout;
}

const uint8_t * audio_data_common_impl::get_raw_data() const
{
    return m_Buffer;
}

dt_data_size_t audio_data_common_impl::get_raw_size() const
{
    return m_DataSize;
}

ptrdiff_t audio_data_common_impl::get_sample_align() const
{
    return utils::audio::bits_to_bytes(get_bits_per_sample()) * get_channels_count();
}

int audio_data_common_impl::get_samples_count() const
{
    return m_SamplesCount;
}

dt_sample_format_t audio_data_common_impl::get_sample_format() const
{
    return m_SampleFormat;
}

dt_channel_layout_t audio_data_common_impl::get_channel_layout() const
{
	return m_ChannelLayout;
}

int audio_data_common_impl::get_channels_count() const
{
    return m_ChannelsCount;
}

int audio_data_common_impl::get_bits_per_sample() const
{
    return m_BitsPerSample;
}

void audio_data_common_impl::resize(size_t _SizeBytes)
{
    if (_SizeBytes <= m_BufferSize)
        return;

    uint8_t * newBuffer = audio_data_common::alloc_buffer(_SizeBytes);
    memcpy(newBuffer, m_Buffer, m_DataSize);
    m_BufferSize = _SizeBytes;
    audio_data_common::free_buffer(m_Buffer);
    m_Buffer = newBuffer;
}

uint8_t * audio_data_common_impl::lock_data()
{
    return m_Buffer;
}

void audio_data_common_impl::unlock_data()
{
}

audio_data_common::audio_data_common(const audio_format * _AudioFormat, uint8_t * _RawBuffer, size_t _SizeBytes, OwnBuffer _OwnBuffer)
{
	audio_data_common_impl * newImpl = new audio_data_common_impl();
	newImpl->set_properties(_AudioFormat->get_sample_rate(),
        _AudioFormat->get_channels_count(),
        _AudioFormat->get_sample_format(),
        _AudioFormat->get_sample_align(),
        _AudioFormat->get_channel_layout());

     newImpl->set_buffer(_RawBuffer, _SizeBytes, _OwnBuffer);

     reset_impl(newImpl);
}

} // namespace openmedia 


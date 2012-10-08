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

/// \file   DTAudioBufferSameFormat.cpp

#include <list>
#include <algorithm>

#include <openmedia/DTError.h>
#include <openmedia/DTAudioData.h>
#include <openmedia/DTAudioFormat.h>

#include "DTAudioBufferSameFormat.h"
#include "DTAudioBufferImpl.h"
#include "DTMemoryBuffer.h"
#include <openmedia/DTAudioDataCommon.h>

namespace openmedia {

/// \class  audio_buffer_same_format_impl
class audio_buffer_same_format_impl: public audio_buffer::Impl
{
public:
    audio_buffer_same_format_impl(const audio_format * _AudioFormat);
    virtual                     ~audio_buffer_same_format_impl();

public:
    virtual void                push_back(const audio_data * _AudioData);
    virtual audio_data_ptr      pop_front(int _SamplesCount); 
    virtual void                push_back_raw(const uint8_t * _Data, size_t _DataSize);
    virtual audio_format_ptr    get_output_audio_format() const;
    virtual int                 get_samples_count() const;

private:
    bool check_format(const audio_data * _AudioData) const;

private:
    details::memory_buffer_simple   m_MemoryBuffer;
    audio_format_ptr                m_AudioFormat;

};

int audio_buffer_same_format_impl::get_samples_count() const
{
    DT_ASSERT(0 != m_AudioFormat->get_sample_align());
    return m_MemoryBuffer.size() / m_AudioFormat->get_sample_align();            
}

audio_buffer_same_format_impl::~audio_buffer_same_format_impl()
{}

bool audio_buffer_same_format_impl::check_format(const audio_data * _AudioData) const
{
    return _AudioData->get_channels_count() == m_AudioFormat->get_channels_count() &&
        _AudioData->get_sample_format() == m_AudioFormat->get_sample_format() && 
        _AudioData->get_channel_layout() == m_AudioFormat->get_channel_layout();

}

audio_buffer_same_format_impl::audio_buffer_same_format_impl(const audio_format * _AudioFormat)
{
    DT_STRONG_ASSERT(NULL != _AudioFormat);
    
    if (!_AudioFormat)
        BOOST_THROW_EXCEPTION(errors::invalid_pointer());

    m_AudioFormat = audio_format::create(
        _AudioFormat->get_sample_rate(),
        _AudioFormat->get_sample_format(),
        _AudioFormat->get_channels_count(),
        _AudioFormat->get_channel_layout()
        );
}

inline void audio_buffer_same_format_impl::push_back(const audio_data * _AudioData)
{
    if (!check_format(_AudioData))
    {
        DT_ASSERT(false && "invalid audio format");
        BOOST_THROW_EXCEPTION(audio_buffer::invalid_format());
    }

    m_MemoryBuffer.push_back(_AudioData->get_raw_data(), _AudioData->get_raw_size());
}

inline audio_data_ptr audio_buffer_same_format_impl::pop_front(int _SamplesCount)
{
    const size_t sizeBytes = _SamplesCount * m_AudioFormat->get_sample_align();
    uint8_t * buffer = audio_data_common::alloc_buffer(sizeBytes);
    size_t bytesReceived = 0;
    m_MemoryBuffer.pop_front(sizeBytes, buffer, bytesReceived);
    return audio_data_ptr(new audio_data_common(m_AudioFormat.get(), buffer, bytesReceived, audio_data_common::bufferGetOwn) );
}

inline void audio_buffer_same_format_impl::push_back_raw(const uint8_t * _Data, size_t _DataSize)
{
    m_MemoryBuffer.push_back(_Data, _DataSize);
}

inline audio_format_ptr audio_buffer_same_format_impl::get_output_audio_format() const
{
    return m_AudioFormat;
}

audio_buffer_same_format::audio_buffer_same_format(const audio_format * _AudioFormat): audio_buffer( new audio_buffer_same_format_impl(_AudioFormat) )
{
    DT_STRONG_ASSERT(NULL != _AudioFormat);    
}

} // namespace openmedia

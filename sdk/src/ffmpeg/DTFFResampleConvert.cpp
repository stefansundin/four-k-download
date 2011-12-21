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


/// \file   DTFFResampleConvert.cpp

#include "DTFFResampleConvert.h"
#include "DTFFSampleFmtConvert.h"

#include <boost/shared_ptr.hpp>

#include "DTFFHeader.h"
#include <openmedia/DTAudioDataCommon.h>
#include "../DTUtils.h"

namespace openmedia {

namespace details {


}

class ff_resample_converter::properties
{
    DT_SIMPLE_PROPERTY(int, filter_length);
    DT_SIMPLE_PROPERTY(int, log2_phase_count);
    DT_SIMPLE_PROPERTY(int, linear);
    DT_SIMPLE_PROPERTY(double, cutoff);

public:
    properties() : m_filter_length(16),
        m_log2_phase_count(10),
        m_linear(0),
        m_cutoff(1.0)
    {}
};

typedef boost::shared_ptr<AVResampleContext> AVResampleContextPtr;

/// \class  ff_resample_converter_impl
class ff_resample_converter_impl : public audio_buffer::Impl
{
public:

    ff_resample_converter_impl(const audio_format * _OutputAudioFormat, int _InputSampleRate, int _OutputSampleRate, const ff_resample_converter::properties * _Properties);
    virtual ~ff_resample_converter_impl();

private:
    virtual void                push_back_impl(const audio_data * _AudioData);
    virtual audio_data_ptr      pop_front_impl(int _SamplesCount); 
    virtual void                push_back_raw_impl(const uint8_t * _Data, size_t _DataSize);
    virtual audio_format_ptr    get_output_audio_format_impl() const;
    virtual int                 get_samples_count_impl() const;

private:
    
    AVResampleContextPtr m_AVResampleContext;

    audio_convert_ptr   m_input_to_s16;
    audio_convert_ptr   m_s16_to_output;

    audio_buffer_ptr    m_s16AudioBufferTail;

    audio_buffer_ptr    m_AudioBuffer;
    audio_format_ptr    m_CurrentInputAudioFormat;
    audio_format_ptr    m_OutputAudioFormat;
    utils::fixed_memory_buffer       m_TempBuffer1;
    utils::fixed_memory_buffer       m_TempBuffer2;
    utils::fixed_memory_buffer       m_TempBufferOut;

    ff_resample_converter::properties m_ResampleProperties;
    int m_InputSampleRate;
    int m_OutputSampleRate;
    double m_Ratio;

};

ff_resample_converter::ff_resample_converter(const audio_format * _OutputAudioFormat, int _InputSampleRate, int _OutputSampleRate) :
audio_convert( new ff_resample_converter_impl(_OutputAudioFormat, _InputSampleRate, _OutputSampleRate, NULL) )
{
}

ff_resample_converter::ff_resample_converter(const audio_format * _OutputAudioFormat, int _InputSampleRate, int _OutputSampleRate, const ff_resample_converter::properties * _Properties) :
audio_convert( new ff_resample_converter_impl(_OutputAudioFormat, _InputSampleRate, _OutputSampleRate, _Properties) )
{
}

namespace details {
class _AVResampleContextDestruct
{
public:
    void operator()(AVResampleContext * _AVResampleContext)
    {
        if (_AVResampleContext)
        {
            av_resample_close(_AVResampleContext);
        }
    }
};

AVResampleContextPtr dt_av_resample_init(int out_rate, int in_rate, int filter_length, int log2_phase_count, int linear, double cutoff)
{
    AVResampleContext * resampleContext = 
        av_resample_init(out_rate, in_rate, filter_length, log2_phase_count, linear, cutoff);
    if (NULL == resampleContext)
    {
        BOOST_THROW_EXCEPTION(errors::bad_alloc());
        return AVResampleContextPtr((AVResampleContext*)NULL, _AVResampleContextDestruct());
    }
    
    return AVResampleContextPtr(resampleContext, _AVResampleContextDestruct());
}

void get_channel_s16(const short * _InputBuffer, size_t _ChannelsCount, size_t _ChannelNum, size_t _SamplesCount, short * _OutputBuffer)
{
    const short * input = _InputBuffer + _ChannelNum;
    short * output = _OutputBuffer;
    size_t stride = _ChannelsCount;

    for(size_t s = 0; s < _SamplesCount; ++s)
    {
        *output++ = *input;    
        input += stride;
    }
}

void mux_channel_s16(short * _OutputBuffer, size_t _ChannelsCount, size_t _ChannelNum, size_t _SamplesCount, short * _InputBuffer)
{
    short * output = _OutputBuffer + _ChannelNum;
    const short * input = _InputBuffer;
    size_t stride = _ChannelsCount;

    for(size_t s = 0; s < _SamplesCount; ++s)
    {
        *output = *input++;    
        output += stride;
    } 
}

}

ff_resample_converter_impl::ff_resample_converter_impl(const audio_format * _OutputAudioFormat, const int _InputSampleRate, const int _OutputSampleRate, const ff_resample_converter::properties * _Properties)
{
    audio_format_ptr outputFormat = audio_format::create( _OutputAudioFormat );
    audio_buffer_ptr audioBuffer = audio_buffer::create(audio_buffer::audio_buffer_SameFormat, _OutputAudioFormat);

    m_OutputAudioFormat = outputFormat;
    m_AudioBuffer = audioBuffer;

    DT_ASSERT(_InputSampleRate > 0);
    DT_ASSERT(_OutputSampleRate > 0);

    audio_format_ptr s16audioFormat = audio_format::create(
        _InputSampleRate,
        DT_SAMPLE_FMT_S16,
        _OutputAudioFormat->get_channels_count(),
        _OutputAudioFormat->get_channel_layout()
        );

    audio_buffer_ptr s16audioBufferTail = audio_buffer::create(audio_buffer::audio_buffer_SameFormat, s16audioFormat.get());

    m_input_to_s16 = audio_convert_ptr( new ff_sample_fmt_converter( s16audioFormat.get() ) );
    m_s16_to_output = audio_convert_ptr( new ff_sample_fmt_converter(_OutputAudioFormat) );

    ff_resample_converter::properties properties = (NULL != _Properties) ? (*_Properties) : (ff_resample_converter::properties());

    m_AVResampleContext = details::dt_av_resample_init(
        _OutputSampleRate,
        _InputSampleRate,
        properties.get_filter_length(),
        properties.get_log2_phase_count(),
        properties.get_linear(),
        properties.get_cutoff() );

    m_InputSampleRate = _InputSampleRate;
    m_OutputSampleRate = _OutputSampleRate;
    m_Ratio = (double)_OutputSampleRate / (double)_InputSampleRate;
    m_s16AudioBufferTail = s16audioBufferTail;

    m_ResampleProperties = properties;
}

ff_resample_converter_impl::~ff_resample_converter_impl()
{
}

void ff_resample_converter_impl::push_back_impl(const audio_data * _AudioData)
{
    DT_STRONG_ASSERT(NULL != _AudioData);
    if (NULL == _AudioData)
        BOOST_THROW_EXCEPTION(errors::invalid_pointer());

    DT_ASSERT(_AudioData->get_samples_count() > 0); // TODO // flush buffer ?
    
    const audio_format * outputAudioFormat = m_OutputAudioFormat.get();
    DT_STRONG_ASSERT(NULL != outputAudioFormat);
    if (_AudioData->get_channels_count() != outputAudioFormat->get_channels_count() ||
        _AudioData->get_channel_layout() != outputAudioFormat->get_channel_layout() // TODO ????
        )
        BOOST_THROW_EXCEPTION( audio_buffer::invalid_format() );

    const int channelsCount = outputAudioFormat->get_channels_count();

    // warning! fixme
    // convert to s16 audio format (for resampling)
    m_input_to_s16->push_back( _AudioData );
    audio_data_ptr s16audioData_tmp = m_input_to_s16->pop_front( m_input_to_s16->get_samples_count() );

    audio_data_ptr s16audioData;
    if (m_s16AudioBufferTail->get_samples_count() > 0)
    {
        m_s16AudioBufferTail->push_back( s16audioData_tmp.get() );
        s16audioData = m_s16AudioBufferTail->pop_front( m_s16AudioBufferTail->get_samples_count() );
    } else
        s16audioData = s16audioData_tmp;

    const size_t leninBytesPerChannel =
        audio_format::size_bytes(m_input_to_s16->get_output_audio_format().get(),
        s16audioData->get_samples_count()) / channelsCount; // TODO
    
    const size_t lenoutBytesPerChannel = 
        audio_format::size_bytes(m_input_to_s16->get_output_audio_format().get(),
        static_cast<size_t>( m_Ratio * s16audioData->get_samples_count() ) ) / channelsCount + 16; // TODO

    const size_t lenoutBytes = channelsCount * lenoutBytesPerChannel; // TODO

    uint8_t * tempBuffer1 = m_TempBuffer1.get_buffer(leninBytesPerChannel);
    uint8_t * tempBuffer2 = m_TempBuffer2.get_buffer(lenoutBytesPerChannel);
    uint8_t * tempBufferOut = m_TempBufferOut.get_buffer(lenoutBytes);

    int predConsumed = 0;
    int predSamplesOut = 0;
    for (int c = 0 ; c < channelsCount ; ++c)
    {
        int consumed;
        details::get_channel_s16(
            (int16_t *)s16audioData->get_raw_data(),
            channelsCount,
            c,
            s16audioData->get_samples_count(),
            (int16_t *)tempBuffer1
            );
        
        int is_last = (c + 1 == channelsCount);
        int samplesOut = av_resample(
            m_AVResampleContext.get(),
            (int16_t*)tempBuffer2,
            (int16_t*)tempBuffer1,
            &consumed,
            s16audioData->get_samples_count(),
            lenoutBytesPerChannel, is_last);
        
        if (c != 0)
        {
            DT_ASSERT(predConsumed == consumed);
            DT_ASSERT(predSamplesOut == samplesOut); 
        }
        else if (s16audioData->get_samples_count() - consumed > 0)
        {
            m_s16AudioBufferTail->push_back_raw( 
                s16audioData->get_raw_data() + s16audioData->get_sample_align() *  consumed,
                (s16audioData->get_samples_count() - consumed) * s16audioData->get_sample_align()
                );
        }
        predConsumed = consumed;
        predSamplesOut = samplesOut;
        details::mux_channel_s16(
            (int16_t*)tempBufferOut,
            channelsCount,
            c,
            samplesOut,
            (int16_t*)tempBuffer2);
    }

    audio_data_ptr convertedData = audio_data_ptr(
        new audio_data_common(
        m_input_to_s16->get_output_audio_format().get(),
        tempBufferOut,
        audio_format::size_bytes(m_input_to_s16->get_output_audio_format().get(), predSamplesOut),
        audio_data_common::bufferRef )
        );

    // convert s16 samples to destination sample format
    m_s16_to_output->push_back(convertedData.get());
    m_AudioBuffer->push_back( m_s16_to_output->pop_front( m_s16_to_output->get_samples_count() ).get() );
    
}

audio_data_ptr ff_resample_converter_impl::pop_front_impl(int _SamplesCount)
{
    return m_AudioBuffer->pop_front(_SamplesCount);
}

void ff_resample_converter_impl::push_back_raw_impl(const uint8_t * _Data, size_t _DataSize)
{
    m_AudioBuffer->push_back_raw(_Data, _DataSize);
}

audio_format_ptr ff_resample_converter_impl::get_output_audio_format_impl() const 
{
    return m_AudioBuffer->get_output_audio_format();
}

int ff_resample_converter_impl::get_samples_count_impl() const
{
    return m_AudioBuffer->get_samples_count();
}

} // namespace openmedia


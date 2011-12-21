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


/// \file   DTFFSampleFmtConvert.cpp

#include "DTFFSampleFmtConvert.h"

#include <boost/shared_ptr.hpp>

#include "DTFFHeader.h"
#include <openmedia/DTAudioDataCommon.h>

namespace openmedia {

///  \class ff_sample_fmt_converter_impl
///  \brief audio sample format converter
class ff_sample_fmt_converter_impl : public audio_buffer::Impl
{
public:
    ff_sample_fmt_converter_impl(const audio_format * _OutputAudioFormat);
    virtual ~ff_sample_fmt_converter_impl();

private:
    virtual void                push_back_impl(const audio_data * _AudioData);
    virtual audio_data_ptr      pop_front_impl(int _SamplesCount); 
    virtual void                push_back_raw_impl(const uint8_t * _Data, size_t _DataSize);
    virtual audio_format_ptr    get_output_audio_format_impl() const;
    virtual int                 get_samples_count_impl() const;

private:

    typedef boost::shared_ptr<AVAudioConvert> AVAudioConvertPtr;
    AVAudioConvertPtr m_AudioConvert;

    audio_buffer_ptr    m_AudioBuffer;
    audio_format_ptr    m_CurrentInputAudioFormat;
    audio_format_ptr    m_OutputAudioFormat;
};

ff_sample_fmt_converter::ff_sample_fmt_converter(const audio_format * _OutputAudioFormat) :
audio_convert( new ff_sample_fmt_converter_impl(_OutputAudioFormat) )
{
}

ff_sample_fmt_converter_impl::ff_sample_fmt_converter_impl(const audio_format * _OutputAudioFormat)
{
    audio_format_ptr outputFormat = audio_format::create( _OutputAudioFormat );
    audio_buffer_ptr audioBuffer = audio_buffer::create(audio_buffer::audio_buffer_SameFormat, _OutputAudioFormat);

    m_OutputAudioFormat = outputFormat;
    m_AudioBuffer = audioBuffer;
}

ff_sample_fmt_converter_impl::~ff_sample_fmt_converter_impl()
{}

namespace details {
class _AVAudioConvertDestruct
{
public:
    void operator()(AVAudioConvert * _AVAudioConvert)
    {
        if (_AVAudioConvert)
        {
            av_audio_convert_free(_AVAudioConvert);
        }
    }
};
    
}

void ff_sample_fmt_converter_impl::push_back_impl(const audio_data * _AudioData)
{
    DT_STRONG_ASSERT(NULL != _AudioData);
    if (NULL == _AudioData)
        BOOST_THROW_EXCEPTION(errors::invalid_pointer());
    
    const audio_format * outputAudioFormat = m_OutputAudioFormat.get();
    DT_STRONG_ASSERT(NULL != outputAudioFormat);
    if (_AudioData->get_channels_count() != outputAudioFormat->get_channels_count() ||
        _AudioData->get_channel_layout() != outputAudioFormat->get_channel_layout() // TODO ????
        )
        BOOST_THROW_EXCEPTION( audio_buffer::invalid_format() );

    if (0 == _AudioData->get_samples_count())
        return;

    if (_AudioData->get_sample_format() == outputAudioFormat->get_sample_format())
    {
        m_AudioBuffer->push_back( _AudioData ); 
        return;
    }

    const audio_format * currentFormat = m_CurrentInputAudioFormat.get();

    if ( !currentFormat || 
        currentFormat->get_sample_format() != _AudioData->get_sample_format()
        )
    {
        audio_format_ptr newAudioFormat = audio_format::create(
            outputAudioFormat->get_sample_rate(),
            _AudioData->get_sample_format(),
            _AudioData->get_channels_count(),
            _AudioData->get_channel_layout() );
        
        m_AudioConvert = AVAudioConvertPtr( 
            
            av_audio_convert_alloc( 
                    DT2FFType(outputAudioFormat->get_sample_format()),
                    1,
                    DT2FFType(_AudioData->get_sample_format()),
                    1,
                    NULL, 0),
                    
                    details::_AVAudioConvertDestruct()                    

                    );

        m_CurrentInputAudioFormat = newAudioFormat;
        currentFormat = m_CurrentInputAudioFormat.get();
    }

    AVAudioConvert * audioConvert = m_AudioConvert.get();
    DT_ASSERT(NULL != audioConvert);
    
    audio_data_ptr convertedData = audio_data_ptr(
        new audio_data_common(outputAudioFormat, NULL, audio_format::size_bytes(outputAudioFormat, _AudioData->get_samples_count() ), audio_data_common::bufferAllocNew )
        );

    void * outBuffer[6] = {(void*)(convertedData->get_raw_data()), NULL, NULL, NULL, NULL, NULL};
    int outStride[6] = {convertedData->get_sample_align() / convertedData->get_channels_count(), 0, 0, 0, 0, 0};

    void * inBuffer[6] = {(void*)(_AudioData->get_raw_data()), NULL, NULL, NULL, NULL, NULL};
    int inStride[6] = {_AudioData->get_sample_align() / _AudioData->get_channels_count() , 0, 0, 0, 0, 0};

    int ffErr = av_audio_convert( audioConvert, outBuffer, outStride, inBuffer, inStride, _AudioData->get_samples_count() * _AudioData->get_channels_count() );
    FF_CHECK_ERR(ffErr);

    m_AudioBuffer->push_back( convertedData.get() );
}

audio_data_ptr ff_sample_fmt_converter_impl::pop_front_impl(int _SamplesCount)
{
    return m_AudioBuffer->pop_front(_SamplesCount);
}

void ff_sample_fmt_converter_impl::push_back_raw_impl(const uint8_t * _Data, size_t _DataSize)
{
    m_AudioBuffer->push_back_raw(_Data, _DataSize);
}

audio_format_ptr ff_sample_fmt_converter_impl::get_output_audio_format_impl() const 
{
    return m_AudioBuffer->get_output_audio_format();
}

int ff_sample_fmt_converter_impl::get_samples_count_impl() const
{
    return m_AudioBuffer->get_samples_count();
}

} // namespace openmedia

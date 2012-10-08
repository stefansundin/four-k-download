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


//  \file   DTFFSWResample.cpp
#include "DTHeadersMedia.h"

#include "DTFFSWResample.h"
#include "DTFFHeader.h"
#include "../DTUtils.h"
#include "../DTAudioBufferImpl.h"
#include <openmedia/DTAudioDataCommon.h>

namespace openmedia {

typedef boost::shared_ptr< ::SwrContext > SwrContextPtr; 

class ff_swresample_impl : public audio_buffer::Impl
{
public:
    ff_swresample_impl(const audio_format * in, const audio_format * out);

public:
    virtual void                push_back(const audio_data * audioData);
    virtual audio_data_ptr      pop_front(int _SamplesCount); 
    virtual void                push_back_raw(const uint8_t * _Data, size_t _DataSize);
    virtual audio_format_ptr    get_output_audio_format() const;
    virtual int                 get_samples_count() const;
    
private:
    SwrContextPtr swresample_context_;
    audio_buffer_ptr audio_buffer_;
    utils::fixed_memory_buffer memory_buffer_;
    audio_format_ptr input_format_;
    audio_format_ptr output_format_;
    double coeff_;

};

ff_swresample::ff_swresample(
                             const audio_format * in,
                             const audio_format * out) : 
audio_convert( new ff_swresample_impl(in, out) )
{    
}

namespace details {

class SwrContextFree_
{
public:
    void operator () (SwrContext * ctx)
    {
        if (ctx)
        {
            SwrContext ** ctx_ = &ctx;
            swr_free(ctx_);
        }
    }
};

}

namespace {

double calc_coeff(  const audio_format * in,
                    const audio_format * out)
{
    return 1.0 * out->get_sample_rate() / in->get_sample_rate() *
        out->get_bits_per_sample() / in->get_bits_per_sample() *
        out->get_channels_count() / in->get_channels_count();
}

int64_t calc_channels_layout(const audio_format * format)
{
    const int64_t layout = format->get_channel_layout() ? 
                               format->get_channel_layout() : 
                               av_get_default_channel_layout(format->get_channels_count());
    return layout;
}

}

SwrContextPtr swr_alloc_set_opts_(const audio_format * in,
                             const audio_format * out)
{
    const int inSampleRate = in->get_sample_rate();
    const int outSampleRate = out->get_sample_rate();
    
    const int64_t inChLayout = calc_channels_layout(in);
    const int64_t outChLayout = calc_channels_layout(out);

    SwrContext * ctx = swr_alloc_set_opts(NULL,
        outChLayout,
        DT2FFType(out->get_sample_format()),
        outSampleRate,
        inChLayout,
        DT2FFType(in->get_sample_format()),
        inSampleRate, 0, NULL);

    if (!ctx)
    {
        BOOST_THROW_EXCEPTION(errors::bad_alloc());
        DT_IF_DISABLE_EXCEPTIONS(return SwrContextPtr());
    }

    SwrContextPtr ctxPtr(ctx, details::SwrContextFree_());

    int dt_err = swr_init(ctx);
    if (dt_err < 0)
    {
        DT_THROW_AVERROR(ff_swresample::error(), dt_err, "swr_init");
        DT_IF_DISABLE_EXCEPTIONS(return SwrContextPtr());
    }
    
    return ctxPtr;
}

ff_swresample_impl::ff_swresample_impl(
        const audio_format * in,
        const audio_format * out) : 
swresample_context_(swr_alloc_set_opts_(in, out)),
audio_buffer_(audio_buffer::create(audio_buffer::audio_buffer_SameFormat, out)),
input_format_( audio_format::create( in ) ),
output_format_( audio_format::create( out ) ),
coeff_(calc_coeff(in, out))
{
}

void ff_swresample_impl::push_back(const audio_data * audioData)
{
    DT_STRONG_ASSERT(!!swresample_context_);
    
    const size_t bytesAvail = (audioData ? audioData->get_raw_size() * coeff_ : 0 ) + 1024;
    uint8_t * buffer = memory_buffer_.get_buffer( bytesAvail );
    const size_t samplesAvail = static_cast<size_t>(1.0 * bytesAvail / 
        output_format_->get_channels_count() / (output_format_->get_bits_per_sample() / 8));
        
    uint8_t * outBuffer[] = {buffer};
    const uint8_t * inBuffer[] = {audioData ? audioData->get_raw_data() : NULL};
    int res = swr_convert(swresample_context_.get(),
        outBuffer,
        samplesAvail,
        audioData ? inBuffer : NULL,
        audioData ? audioData->get_samples_count() : 0); 

#ifdef DT_DEBUG
    const int inputSamplesCount = (audioData ? audioData->get_samples_count() : 0);
    double dbgCoeff = (inputSamplesCount ? 1.0 * res / inputSamplesCount : 0.0);
#endif

    if (res < 0)
    {
        DT_THROW_AVERROR(ff_swresample::error(), res, "swr_convert"); 
        return;
    }

    audio_data_ptr convertedData = audio_data_ptr(
        new audio_data_common(
        output_format_.get(),
        buffer,
        audio_format::size_bytes(output_format_.get(), res),
        audio_data_common::bufferRef )
        );
    audio_buffer_->push_back(convertedData.get());
}

audio_data_ptr ff_swresample_impl::pop_front(int _SamplesCount)
{
    return audio_buffer_->pop_front(_SamplesCount);
}

void ff_swresample_impl::push_back_raw(const uint8_t * _Data, size_t _DataSize)
{
    audio_buffer_->push_back_raw(_Data, _DataSize);
}

audio_format_ptr ff_swresample_impl::get_output_audio_format() const
{
    return audio_buffer_->get_output_audio_format();
}

int ff_swresample_impl::get_samples_count() const
{
    return audio_buffer_->get_samples_count();
}


}

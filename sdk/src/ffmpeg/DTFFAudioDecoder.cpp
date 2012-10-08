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

/// \file   DTFFAudioDecoder.cpp

#include "DTFFAudioDecoder.h"

#include <list>

#define DT_FF_NEED_AVCODEC
#include "DTFFHeader.h"

#include <openmedia/DTCommon.h>
#include <openmedia/DTError.h>

#include <openmedia/DTAudioBuffer.h>
#include <openmedia/DTAudioFormat.h>
#include <openmedia/DTAudioDecoder.h>
#include <openmedia/DTAudioDataCommon.h>
#include <openmedia/DTAudioDataTimed.h>
#include <openmedia/DTAudioDecoderInfo.h>
#include <openmedia/DTMP3File.h>

#include "../DTAudioDecoderImpl.h"

#include "DTFFCodec.h"
#include "DTFFDecoderInfo.h"
#include "DTFFAlloc.h"

namespace openmedia {

/// \class  audio_decoder_additional_info_impl
class audio_decoder_additional_info_impl : public audio_decoder_additional_info::Impl
{
public:
    audio_decoder_additional_info_impl(const std::string & _CodecName, const std::string & _CodecLongName) :
      m_CodecName(_CodecName), m_CodecLongName(_CodecLongName)
    {}

public:
    virtual std::string get_codec_name() const
    {
        return m_CodecName;
    }

    virtual std::string get_codec_long_name() const
    {
        return m_CodecLongName;
    }

private:
    std::string m_CodecName;
    std::string m_CodecLongName;

};

class audio_decoder_additional_info_ : public audio_decoder_additional_info
{
public:
    audio_decoder_additional_info_(const std::string & _CodecName, const std::string & _CodecLongName) :
      audio_decoder_additional_info( new audio_decoder_additional_info_impl( _CodecName, _CodecLongName) ) 
      {}
};

/// \class  ff_audio_decoder_impl
class ff_audio_decoder_impl: public audio_decoder::Impl
{
public:
    ff_audio_decoder_impl(const audio_decoder_info * _AudioDecoderInfo);

public:
    virtual audio_data_ptr          decode(media_packet_ptr _MediaPacket);    
    virtual audio_data_timed_ptr    decode_timed(media_packet_ptr _MediaPacket);
    virtual audio_decoder_info_ptr  get_decoder_info() const;
    virtual audio_decoder_additional_info_ptr get_additional_info() const;
    virtual void reset();

private:
    std::list<media_packet_ptr>         m_AudioDataList;
    AVCodecContextPtr                   m_CodecContextPtr;
    audio_format_ptr                    m_AudioFormat;
    audio_buffer_ptr                    m_AudioBuffer;
    dt_float_t                          m_audioClock;

private:
    audio_decoder_additional_info_ptr   m_AdditionalInfo;

#ifdef _MSC_VER
    __declspec(align(16)) uint8_t m_audio_buf[(AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2 + 16];
#else
    uint8_t m_audio_buf[(AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2] __attribute__ ((aligned (16)));
#endif

    uint8_t * m_audio_buf_aligned;

};

ff_audio_decoder::ff_audio_decoder(const audio_decoder_info * _AudioDecoderInfo): audio_decoder( new ff_audio_decoder_impl(_AudioDecoderInfo) )
{
}

void ff_audio_decoder_impl::reset()
{
    avcodec_flush_buffers(m_CodecContextPtr.get());
}

audio_decoder_info_ptr ff_audio_decoder_impl::get_decoder_info() const
{
    return audio_decoder_info_ptr( new ff_audio_decoder_info(m_CodecContextPtr.get()) );
}

ff_audio_decoder_impl::ff_audio_decoder_impl(const audio_decoder_info * _AudioDecoderInfo)
    : m_audioClock(0.0), m_audio_buf_aligned(  (uint8_t *)( (intptr_t)(((uint8_t *)(m_audio_buf) + 16)) & ~ 0x0F )  )
{
    DT_ASSERT(NULL != _AudioDecoderInfo);
    const audio_decoder_info * adi = _AudioDecoderInfo;
    AVCodecContext * ctx = avcodec_alloc_context();

    ctx->codec_id       = DT2FFType(adi->get_codec_id());
#ifdef _MSC_VER
#   pragma warning(push)
#   pragma warning(disable:4996)
#endif
    adi->get_codec_name().copy(ctx->codec_name, sizeof(ctx->codec_name) - 1);
#ifdef _MSC_VER
#   pragma warning(pop)
#endif
    ctx->codec_name[sizeof(ctx->codec_name) - 1] = 0;
    ctx->codec_tag      = adi->get_codec_tag();
    ctx->codec_type     = DT2FFType(adi->get_codec_type());
    ctx->extradata      = NULL;
    ctx->extradata_size = 0;
    if (adi->get_extradata_size() > 0 && adi->get_extradata() != NULL)
    {
        ff_alloc_and_copy(&ctx->extradata, adi->get_extradata(), adi->get_extradata_size(), FF_INPUT_BUFFER_PADDING_SIZE);
        ctx->extradata_size = adi->get_extradata_size();
    }

    ctx->profile        = adi->get_profile();
    ctx->level          = adi->get_level();
    ctx->bit_rate       = adi->get_bit_rate();
    ctx->time_base      = DT2FFType(adi->get_time_base());

    ctx->sample_rate    = adi->get_sample_rate();
    ctx->channels       = adi->get_channels_count();
    ctx->sample_fmt     = DT2FFType(adi->get_sample_format());
    ctx->channel_layout = adi->get_channel_layout();
    ctx->block_align    = adi->get_sample_align();
    ctx->bits_per_coded_sample = adi->get_bits_per_coded_sample();
    ctx->bits_per_raw_sample = adi->get_bits_per_raw_sample();

    dt_open_ff_decoder(ctx, FF2DTType(ctx->codec_id));

    m_AdditionalInfo = 
        audio_decoder_additional_info_ptr( new audio_decoder_additional_info_ ( ctx->codec->name, ctx->codec->long_name ) );

    audio_format_ptr audioFormat = audio_format::create(adi->get_sample_rate(),
        adi->get_sample_format(),
        adi->get_channels_count(),
        adi->get_channel_layout());

    audio_buffer_ptr audioBuffer = audio_buffer::create(audio_buffer::audio_buffer_SameFormat, audioFormat.get());

    m_AudioFormat = audioFormat;
    m_AudioBuffer = audioBuffer;
    m_CodecContextPtr = dt_create_codec_context(ctx);

}

audio_decoder_additional_info_ptr ff_audio_decoder_impl::get_additional_info() const
{
    return m_AdditionalInfo;
}

audio_data_ptr ff_audio_decoder_impl::decode(media_packet_ptr _MediaPacket)
{
    //media_packet_ptr dupMediaPacket     = _MediaPacket->duplicate();
    //m_AudioDataList.push_back(dupMediaPacket);

    //const media_packet * packet         =  dupMediaPacket.get();

    audio_buffer * audioBuffer = m_AudioBuffer.get();
    const media_packet * packet = _MediaPacket.get();
    AVCodecContext * context = m_CodecContextPtr.get();
    
    media_packet_ptr dupPacket = packet->duplicate();

    uint8_t * data = const_cast<uint8_t *>(dupPacket->get_data());
    dt_data_size_t size = dupPacket->get_size();

    while (size > 0)
    {
        AVPacket avPacket;
        av_init_packet(&avPacket);
        avPacket.data = const_cast<uint8_t *>(data);
        avPacket.size = size;
        int data_size = sizeof(m_audio_buf) - 16;
        
        int len = avcodec_decode_audio3(context, (int16_t*)(m_audio_buf_aligned), &data_size, &avPacket);

        if (len < 0)
        {
            DT_ASSERT(false);
            break;
            // TODO: error;
        }

        data += len;
        size -= len;

        if (data_size <= 0)
            continue;
        audioBuffer->push_back_raw(m_audio_buf_aligned, data_size);
    }

    return audioBuffer->pop_front( audioBuffer->get_samples_count() );
}

audio_data_timed_ptr ff_audio_decoder_impl::decode_timed(media_packet_ptr _MediaPacket)
{
    if ( _MediaPacket->get_pts() != AV_NOPTS_VALUE )
    {
        m_audioClock = av_q2d(m_CodecContextPtr->time_base) * _MediaPacket->get_pts();
    }

    audio_data_ptr audioData = decode(_MediaPacket);
    dt_float_t pts = m_audioClock;

    double duration = 0.0;
    if (m_CodecContextPtr->sample_rate)
    {
        duration = 1.0 * audioData->get_samples_count() / m_CodecContextPtr->sample_rate;
        m_audioClock += duration;
    }

    return audio_data_timed_ptr(new audio_data_timed(audioData, duration, pts));
}


namespace details {

/// \class  _audio_decoder  
class _audio_decoder : boost::noncopyable
{
    _audio_decoder() : m_Samples(NULL),
        m_SamplesSize(0)
    {}

    ~_audio_decoder();
    void    reset();
    void    dt_avcodec_decode_audio3(AVCodecContext * _CodecContext, AVPacket * _Packet, int * _FrameSize, int * _UsedDataSize);

    inline const dt_auido_sample_t * get_output_samples()
    {
        return m_Samples;    
    }

private:
    void reset_buffer();

    inline dt_auido_sample_t * get_buffer(int _PacketBufferSize)
    {
        const size_t samplesSize = (std::max)((size_t)(_PacketBufferSize * sizeof(*m_Samples)), (size_t)AVCODEC_MAX_AUDIO_FRAME_SIZE);
        return (m_SamplesSize < samplesSize) ? reallocate_buffer(samplesSize) : m_Samples;
    }

    dt_auido_sample_t * reallocate_buffer(size_t _SamplesSize);

private:
    dt_auido_sample_t * m_Samples;
    size_t              m_SamplesSize;
};

_audio_decoder::~_audio_decoder()
{
    reset();
}

dt_auido_sample_t * _audio_decoder::reallocate_buffer(size_t _SamplesSize)
{
    dt_auido_sample_t * newBuffer = NULL;
    newBuffer = (dt_auido_sample_t *)av_malloc(_SamplesSize);
    if (_SamplesSize && (NULL == newBuffer))
        BOOST_THROW_EXCEPTION(errors::bad_alloc());

    av_free((void*)m_Samples);
    m_Samples = newBuffer;
    m_SamplesSize = _SamplesSize; 
    return m_Samples;
}

void _audio_decoder::reset_buffer()
{
    if (NULL != m_Samples)
        av_free(m_Samples);
    m_Samples = NULL;
    m_SamplesSize = 0;
}

void _audio_decoder::dt_avcodec_decode_audio3(AVCodecContext * _CodecContext, AVPacket * _Packet, int * _FrameSize, int * _UsedDataSize)
{
    DT_ASSERT(!(NULL == _CodecContext || NULL == _FrameSize));
    if (NULL == _CodecContext || NULL == _FrameSize)
    {
        BOOST_THROW_EXCEPTION(errors::invalid_argument());		
    }

    int frameSize = 0;
    int ret = avcodec_decode_audio3(_CodecContext, get_buffer(_Packet->size), &frameSize, _Packet);
    if (FF_ERR(ret))
    {
        DT_THROW_AVERROR(audio_decoder::decode_error(), ret, "avcodec_decode_audio3");
    }

    *_UsedDataSize = ret;
    *_FrameSize = frameSize;
}

void _audio_decoder::reset()
{
    reset_buffer();
}

}


} // namespace openmedia

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

/// \file   DTFFDecoderInfo.cpp

#include "DTFFDecoderInfo.h"

#include <openmedia/DTError.h>

#define DT_FF_NEED_AVCODEC
#include "DTFFHeader.h"

#include "DTFFAlloc.h"

#include "../DTDecoderInfoImpl.h"
#include "../DTAudioDecoderInfoImpl.h"
#include "../DTVideoDecoderInfoImpl.h"

namespace openmedia {

class av_decoder_info_impl: public decoder_info::Impl
{
public:
    virtual ~av_decoder_info_impl()
    {
        av_freep(&m_CodecContext.extradata);
    }

public:
    virtual const uint8_t * get_extradata() const
    {
        return m_CodecContext.extradata;
    }

    virtual int             get_extradata_size() const
    {
        return m_CodecContext.extradata_size;
    }

    virtual std::string     get_codec_name() const
    {
        return std::string(m_CodecContext.codec_name);
    }

    virtual dt_codec_base_t get_codec_base() const
    {
        return CODEC_BASE_FFMPEG;
    }

    virtual dt_media_type_t get_codec_type() const
    {
        return FF2DTType(m_CodecContext.codec_type);
    }

    virtual dt_codec_id_t   get_codec_id() const
    {
        return FF2DTType(m_CodecContext.codec_id);
    }

    virtual unsigned int    get_codec_tag() const
    {
        return m_CodecContext.codec_tag;
    }

    virtual int             get_profile() const
    {
        return m_CodecContext.profile;
    }

    virtual int             get_level() const
    {
        return m_CodecContext.level;
    }

    virtual int             get_bit_rate() const
    {
        return m_CodecContext.bit_rate;
    }

    virtual dt_rational_t get_time_base() const
    {
        return FF2DTType(m_CodecContext.time_base);
    }

    virtual int get_rc_max_rate() const
    {
        return m_CodecContext.rc_max_rate;
    }

    virtual int get_rc_buffer_size() const
    {
        return m_CodecContext.rc_buffer_size;
    }

private:
    AVCodecContext m_CodecContext;

public:
    av_decoder_info_impl(const AVCodecContext * _CodecContext)
    {
        AVCodecContext * ctx = &m_CodecContext;

        ff_alloc_and_copy(&ctx->extradata, _CodecContext->extradata, _CodecContext->extradata_size, FF_INPUT_BUFFER_PADDING_SIZE);
        ctx->extradata_size = _CodecContext->extradata_size;
        memcpy(ctx->codec_name, _CodecContext->codec_name, sizeof(_CodecContext->codec_name));
        ctx->codec_type = _CodecContext->codec_type;
        ctx->codec_id = _CodecContext->codec_id;
        ctx->codec_tag = _CodecContext->codec_tag;
        ctx->profile = _CodecContext->profile;
        ctx->level = _CodecContext->level;
        ctx->time_base = _CodecContext->time_base;
        ctx->bit_rate = _CodecContext->bit_rate;
        ctx->rc_max_rate = _CodecContext->rc_max_rate;
        ctx->rc_buffer_size = _CodecContext->rc_buffer_size;

    }
};

/// \class  av_decoder_info_impl
class av_video_decoder_info_impl: public video_decoder_info::Impl
{
public:
    virtual const uint8_t * get_extradata() const
    {
        return m_CodecContext.extradata;
    }

    virtual int get_extradata_size() const
    {
        return m_CodecContext.extradata_size;
    }

    virtual std::string get_codec_name() const
    {
        return std::string(m_CodecContext.codec_name);
    }

    virtual dt_codec_base_t get_codec_base() const
    {
        return CODEC_BASE_FFMPEG;
    }

    virtual dt_media_type_t get_codec_type() const
    {
        return FF2DTType(m_CodecContext.codec_type);
    }

    virtual dt_codec_id_t get_codec_id() const
    {
        return FF2DTType(m_CodecContext.codec_id);
    }

    virtual unsigned int get_codec_tag() const
    {
        return m_CodecContext.codec_tag;
    }

    virtual int get_profile() const
    {
        return m_CodecContext.profile;
    }

    virtual int get_level() const
    {
        return m_CodecContext.level;
    }

    virtual int get_bit_rate() const
    {
        return m_CodecContext.bit_rate;
    }

    virtual dt_rational_t   get_time_base() const
    {
        return FF2DTType(m_CodecContext.time_base);
    }

    virtual int get_rc_max_rate() const
    {
        return m_CodecContext.rc_max_rate;
    }

    virtual int get_rc_buffer_size() const
    {
        return m_CodecContext.rc_buffer_size;
    }

public:
    virtual int get_width() const
    {
        return m_CodecContext.width;
    }

    virtual int get_height() const
    {
        return m_CodecContext.height;
    }

    virtual dt_pixel_format_t get_pix_fmt() const
    {
        return FF2DTType(m_CodecContext.pix_fmt);
    }

    virtual bool has_b_frames() const
    {
        return 0 != m_CodecContext.has_b_frames;
    }

    virtual dt_rational_t get_sample_aspect_ratio() const
    {
        return FF2DTType(m_CodecContext.sample_aspect_ratio);
    }

    virtual const uint16_t * get_intra_matrix() const
    {
        return m_CodecContext.intra_matrix;
    }

    virtual const uint16_t * get_inter_matrix() const
    {
        return m_CodecContext.inter_matrix;
    }

    virtual int get_reference_frames() const
    {
        return m_CodecContext.refs;
    }
    
    virtual int get_ticks_per_frame() const
    {
        return m_CodecContext.ticks_per_frame;
    }

private:
    AVCodecContext m_CodecContext;

public:
    av_video_decoder_info_impl(const AVCodecContext * _CodecContext)
    {
        AVCodecContext * ctx = &m_CodecContext;

        ff_alloc_and_copy(&ctx->extradata, _CodecContext->extradata, _CodecContext->extradata_size, FF_INPUT_BUFFER_PADDING_SIZE);
        ctx->extradata_size = _CodecContext->extradata_size;
        memcpy(ctx->codec_name, _CodecContext->codec_name, sizeof(_CodecContext->codec_name));
        ctx->codec_type = _CodecContext->codec_type;
        ctx->codec_id = _CodecContext->codec_id;
        ctx->codec_tag = _CodecContext->codec_tag;
        ctx->profile = _CodecContext->profile;
        ctx->level = _CodecContext->level;
        ctx->time_base = _CodecContext->time_base;
        ctx->bit_rate = _CodecContext->bit_rate;
        ctx->rc_max_rate = _CodecContext->rc_max_rate;
        ctx->rc_buffer_size = _CodecContext->rc_buffer_size;
        
        ctx->width = _CodecContext->width;
        ctx->height = _CodecContext->height;
        ctx->pix_fmt = _CodecContext->pix_fmt;
        ctx->has_b_frames = _CodecContext->has_b_frames;
        ctx->sample_aspect_ratio = _CodecContext->sample_aspect_ratio;
        ctx->ticks_per_frame = _CodecContext->ticks_per_frame;
    
        ff_alloc_and_copy((uint8_t**)(&ctx->inter_matrix), (uint8_t*)_CodecContext->inter_matrix, 64 * sizeof(int16_t), 0);
        ff_alloc_and_copy((uint8_t**)(&ctx->intra_matrix), (uint8_t*)_CodecContext->intra_matrix, 64 * sizeof(int16_t), 0);

        ctx->refs = _CodecContext->refs;
    }
};

/// \class  av_audio_decoder_info_impl
class av_audio_decoder_info_impl: public audio_decoder_info::Impl
{
private:
    virtual const uint8_t * get_extradata() const
    {
        return m_CodecContext.extradata;
    }

    virtual int get_extradata_size() const
    {
        return m_CodecContext.extradata_size;
    }

    virtual std::string get_codec_name() const
    {
        return std::string(m_CodecContext.codec_name);
    }

    virtual dt_codec_base_t get_codec_base() const
    {
        return CODEC_BASE_FFMPEG;
    }

    virtual dt_media_type_t get_codec_type() const
    {
        return FF2DTType(m_CodecContext.codec_type);
    }

    virtual dt_codec_id_t get_codec_id() const
    {
        return FF2DTType(m_CodecContext.codec_id);
    }

    virtual unsigned int get_codec_tag() const
    {
        return m_CodecContext.codec_tag;
    }

    virtual int get_profile() const
    {
        return m_CodecContext.profile;
    }

    virtual int get_level() const
    {
        return m_CodecContext.level;
    }

    virtual int get_bit_rate() const
    {
        return m_CodecContext.bit_rate;
    }

    virtual dt_rational_t get_time_base() const
    {
        return FF2DTType(m_CodecContext.time_base);
    }

    virtual int get_rc_max_rate() const
    {
        return m_CodecContext.rc_max_rate;
    }

    virtual int get_rc_buffer_size() const
    {
        return m_CodecContext.rc_buffer_size;
    }

public:
    virtual int get_sample_rate() const
    {
        return m_CodecContext.sample_rate;
    }

    virtual int get_channels_count() const
    {
        return m_CodecContext.channels;
    }

    virtual dt_sample_format_t get_sample_format() const
    {
        return FF2DTType(m_CodecContext.sample_fmt);
    }

    virtual int get_block_align() const
    {
        return m_CodecContext.block_align;
    }

    virtual dt_channel_layout_t get_channel_layout() const
    {
        return m_CodecContext.channel_layout;
    }

    virtual int get_bits_per_coded_sample() const
    {
        return m_CodecContext.bits_per_coded_sample;
    }
    
    virtual int get_bits_per_raw_sample() const
    {
        return m_CodecContext.bits_per_raw_sample;
    }

private:
    AVCodecContext m_CodecContext;

public:
    av_audio_decoder_info_impl(const AVCodecContext * _CodecContext)
    {
        DT_STRONG_ASSERT(NULL != _CodecContext);
        if (NULL == _CodecContext)
        {
            BOOST_THROW_EXCEPTION(errors::invalid_argument());
        }

        AVCodecContext * ctx = &m_CodecContext;

        ff_alloc_and_copy(&ctx->extradata, _CodecContext->extradata, _CodecContext->extradata_size, FF_INPUT_BUFFER_PADDING_SIZE);
        ctx->extradata_size = _CodecContext->extradata_size;
        memcpy(ctx->codec_name, _CodecContext->codec_name, sizeof(_CodecContext->codec_name));
        ctx->codec_type = _CodecContext->codec_type;
        ctx->codec_id = _CodecContext->codec_id;
        ctx->codec_tag = _CodecContext->codec_tag;
        ctx->profile = _CodecContext->profile;
        ctx->level = _CodecContext->level;
        ctx->bit_rate = _CodecContext->bit_rate;
        ctx->time_base = _CodecContext->time_base;
        ctx->rc_max_rate = _CodecContext->rc_max_rate;
        ctx->rc_buffer_size = _CodecContext->rc_buffer_size;

        ctx->sample_rate = _CodecContext->sample_rate;
        ctx->channels = _CodecContext->channels;
        ctx->sample_fmt = _CodecContext->sample_fmt;
        ctx->block_align = _CodecContext->block_align;
        ctx->channel_layout = _CodecContext->channel_layout;
        ctx->bits_per_coded_sample = _CodecContext->bits_per_coded_sample;
        ctx->bits_per_raw_sample = _CodecContext->bits_per_raw_sample;
    }

};

///

ff_decoder_info::ff_decoder_info(const AVCodecContext * _CodecContext): decoder_info( new av_decoder_info_impl(_CodecContext) )
{
    DT_ASSERT(NULL != _CodecContext);
    if (NULL == _CodecContext)
    {
        BOOST_THROW_EXCEPTION(errors::invalid_argument());
    }
}

ff_video_decoder_info::ff_video_decoder_info(const AVCodecContext * _CodecContext): video_decoder_info( new av_video_decoder_info_impl(_CodecContext) )
{
    DT_ASSERT(NULL != _CodecContext);
    if (NULL == _CodecContext)
    {
        BOOST_THROW_EXCEPTION(errors::invalid_argument());
    }
}

ff_audio_decoder_info::ff_audio_decoder_info(const AVCodecContext * _CodecContext): audio_decoder_info( new av_audio_decoder_info_impl(_CodecContext) )
{
    DT_ASSERT(NULL != _CodecContext);
    if (NULL == _CodecContext)
    {
        BOOST_THROW_EXCEPTION(errors::invalid_argument());
    }
}

decoder_info_ptr create_ff_decoder_info(AVCodecContext * _Codec)
{
    DT_ASSERT(NULL != _Codec);
    if (NULL == _Codec)
    {
        BOOST_THROW_EXCEPTION(errors::invalid_pointer());
        DT_IF_DISABLE_EXCEPTIONS(return decoder_info_ptr());
    }

    decoder_info_ptr decoderInfo;
    switch (_Codec->codec_type)
    {
    case AVMEDIA_TYPE_VIDEO:
        decoderInfo = ff_video_decoder_info::create(_Codec);
        break;
    case AVMEDIA_TYPE_AUDIO:
        decoderInfo = ff_audio_decoder_info::create(_Codec);
        break;
    default:
        decoderInfo = ff_decoder_info::create(_Codec);
        break;
    }
    return decoderInfo;
}

} // namespace openmedia

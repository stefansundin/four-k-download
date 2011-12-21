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


/// \file  DTFFVideoDecoder.cpp

#include "DTFFVideoDecoder.h"

#include <openmedia/DTCodec.h>
#include <openmedia/DTError.h>
#include <openmedia/DTVideoData.h>
//#include <openmedia/DTUtils.h>
#include <openmedia/DTAdditionalSettings.h>
#include <openmedia/DTVideoDataTimed.h>
#include <openmedia/DTVideoDecoder.h>
#include <openmedia/DTVideoDecoderInfo.h>

#include "../DTVideoDataImpl.h"
#include "../DTVideoDecoderImpl.h"
#include "../DTVideoDecoderInfoImpl.h"

#include "DTFFVideoData.h"
#include "DTFFDecoderInfo.h"
#include "DTFFCodec.h"
#include "DTFFPicture.h"
#include "DTFFHeader.h"
#include "DTFFAlloc.h"

namespace openmedia {

/// \class  audio_decoder_additional_info_impl
class video_decoder_additional_info_impl : public video_decoder_additional_info::Impl
{
public:
    video_decoder_additional_info_impl(const std::string & _CodecName, const std::string & _CodecLongName) :
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

class video_decoder_additional_info_ : public video_decoder_additional_info
{
public:
    video_decoder_additional_info_(const std::string & _CodecName, const std::string & _CodecLongName) :
      video_decoder_additional_info( new video_decoder_additional_info_impl( _CodecName, _CodecLongName) ) 
      {}
};

/// \class  ff_video_decoder_impl
class ff_video_decoder_impl: public video_decoder::Impl
{
public:
    virtual ~ff_video_decoder_impl();

private:
    virtual video_data_ptr          decode(media_packet_ptr _MediaPacket);    
    virtual video_data_timed_ptr    decode_timed(media_packet_ptr _MediaPacket);
    virtual video_decoder_info_ptr  get_decoder_info() const;    
    virtual video_decoder_additional_info_ptr get_additional_info() const;

private:

    AVCodecContextPtr m_CodecContextPtr;
    AVFrame * m_Frame;
    video_decoder_additional_info_ptr   m_AdditionalInfo;
    dt_float_t m_videoClock;

private:
    static void ParseAdditionalSettings(AVCodecContext * _CodecContext, const additional_settings * _AdditionalSettings);

public:
    ff_video_decoder_impl(video_decoder_info * _VideoDecoderInfo, const additional_settings * _AdditionalSettings);

};

ff_video_decoder::ff_video_decoder(video_decoder_info * _VideoDecoderInfo, const additional_settings * _AdditionalSettings): video_decoder( new ff_video_decoder_impl(_VideoDecoderInfo, _AdditionalSettings) )
{
}

///

void ff_video_decoder_impl::ParseAdditionalSettings(AVCodecContext * _CodecContext, const additional_settings * _AdditionalSettings)
{
    AVCodecContextAdditionalSettings(_CodecContext, _AdditionalSettings);
}

ff_video_decoder_impl::ff_video_decoder_impl(video_decoder_info * _VideoDecoderInfo, const additional_settings * _AdditionalSettings)
    : m_videoClock(0)
{
    video_decoder_info * vdi = _VideoDecoderInfo;
    DT_ASSERT(NULL != vdi);
    
    AVCodecContext * ctx = avcodec_alloc_context();
    DT_ASSERT(NULL != ctx);
    
    ctx->codec_id       = DT2FFType(vdi->get_codec_id());
#ifdef _MSC_VER
#   pragma warning(push)
#   pragma warning(disable:4996)
#endif
    vdi->get_codec_name().copy(ctx->codec_name, sizeof(ctx->codec_name) - 1);
#ifdef _MSC_VER
#   pragma warning(pop)
#endif
    ctx->codec_name[sizeof(ctx->codec_name) - 1] = 0;
    ctx->codec_tag      = vdi->get_codec_tag();
    ctx->codec_type     = DT2FFType(vdi->get_codec_type());
    ctx->extradata      = NULL;
    ctx->extradata_size = 0;
    if (vdi->get_extradata_size() > 0 && vdi->get_extradata() != NULL)
    {
        ff_alloc_and_copy(&ctx->extradata, vdi->get_extradata(), vdi->get_extradata_size(), FF_INPUT_BUFFER_PADDING_SIZE);
        ctx->extradata_size = vdi->get_extradata_size();
    }

    ff_alloc_and_copy((uint8_t**)(&ctx->inter_matrix), (uint8_t*)vdi->get_inter_matrix(), 64 * sizeof(int16_t), 0);
    ff_alloc_and_copy((uint8_t**)(&ctx->intra_matrix), (uint8_t*)vdi->get_intra_matrix(), 64 * sizeof(int16_t), 0);
    
    ctx->width      = vdi->get_width();
    ctx->height     = vdi->get_height();
    ctx->profile    = vdi->get_profile();
    ctx->level      = vdi->get_level();
    ctx->bit_rate   = vdi->get_bit_rate();
    ctx->time_base  = DT2FFType(vdi->get_time_base());

    ctx->pix_fmt    = DT2FFType(vdi->get_pix_fmt());
    ctx->refs       = vdi->get_reference_frames();
    ctx->sample_aspect_ratio    = DT2FFType(vdi->get_sample_aspect_ratio());
    ctx->has_b_frames           = vdi->has_b_frames();
    ctx->ticks_per_frame        = vdi->get_ticks_per_frame();

    if (_AdditionalSettings)
        ParseAdditionalSettings(ctx, _AdditionalSettings);

    dt_open_ff_decoder(ctx, FF2DTType(ctx->codec_id));

    m_AdditionalInfo = 
        video_decoder_additional_info_ptr( new video_decoder_additional_info_ ( ctx->codec->name, ctx->codec->long_name ) );

    m_CodecContextPtr = dt_create_codec_context(ctx);
}

video_decoder_additional_info_ptr ff_video_decoder_impl::get_additional_info() const
{
    return m_AdditionalInfo;
}

video_decoder_info_ptr ff_video_decoder_impl::get_decoder_info() const
{
    return video_decoder_info_ptr( new ff_video_decoder_info(m_CodecContextPtr.get()));
}


ff_video_decoder_impl::~ff_video_decoder_impl()
{
}

video_data_ptr   ff_video_decoder_impl::decode(media_packet_ptr _MediaPacket)
{
    DT_ASSERT(NULL != m_CodecContextPtr.get());
    int gotPicture;
    AVFramePtr outputFrame = dt_create_decoded_frame();

    AVPacket avPacket;
    av_init_packet(&avPacket);
    avPacket.data = const_cast<uint8_t *>(_MediaPacket->get_data());
    avPacket.size = _MediaPacket->get_size();
    
    int res = avcodec_decode_video2(m_CodecContextPtr.get(), outputFrame.get(), &gotPicture, &avPacket);

    FF_CHECK_ERR(res);
    if FF_ERR(res)
    {
        DT_THROW_AVERROR(decoder::decode_error(), res, "avcodec_decode_video");
        return video_data_ptr((video_data*) NULL);
    }

    video_data_ptr videoData( new ff_video_data(outputFrame, m_CodecContextPtr->width, m_CodecContextPtr->height, FF2DTType(m_CodecContextPtr->pix_fmt)) );
    if (NULL != videoData->get_data())
    {
        return videoData->duplicate();
    } else
        return video_data_ptr((video_data*) NULL);
}

video_data_timed_ptr ff_video_decoder_impl::decode_timed(media_packet_ptr _MediaPacket)
{
    DT_ASSERT(NULL != m_CodecContextPtr.get());
    int gotPicture;
    AVFramePtr outputFrame = dt_create_decoded_frame();
    dt_ts_t intPts = 0;
    dt_float_t pts = 0.0;

    m_CodecContextPtr->reordered_opaque = _MediaPacket->get_pts();

    AVPacket avPacket;
    av_init_packet(&avPacket);
    avPacket.data = const_cast<uint8_t *>(_MediaPacket->get_data());
    avPacket.size = _MediaPacket->get_size();
    
    int res = avcodec_decode_video2(m_CodecContextPtr.get(), outputFrame.get(), &gotPicture, &avPacket);
    FF_CHECK_ERR(res);
    if FF_ERR(res)
    {
        DT_THROW_AVERROR(decoder::decode_error(), res, "avcodec_decode_video");
        return video_data_timed_ptr((video_data_timed*) NULL);
    }

    if ( ! gotPicture )
        return video_data_timed_ptr((video_data_timed*) NULL);

    if ( _MediaPacket->get_dts() != AV_NOPTS_VALUE )
        intPts = _MediaPacket->get_dts();
    else if ( outputFrame->reordered_opaque != AV_NOPTS_VALUE )
        intPts = outputFrame->reordered_opaque;

    pts = intPts * _MediaPacket->get_time_base().to_double();

    if ( pts != 0 )
       m_videoClock = pts;
    else
        pts = m_videoClock;

    dt_float_t frameDelay = av_q2d(m_CodecContextPtr->time_base);
    frameDelay += outputFrame->repeat_pict * (frameDelay * 0.5);
    m_videoClock += frameDelay;

    video_data_ptr videoData(new ff_video_data(outputFrame, m_CodecContextPtr->width, m_CodecContextPtr->height, FF2DTType(m_CodecContextPtr->pix_fmt)) );

    if (videoData && videoData->get_data())
        return video_data_timed_ptr(new video_data_timed(videoData->duplicate(), 0, pts));
    else
        return video_data_timed_ptr((video_data_timed*) NULL);
}


} // namespace openmedia 


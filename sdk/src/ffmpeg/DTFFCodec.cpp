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

#include "DTFFCodec.h"

#include <openmedia/DTError.h>

#define DT_FF_NEED_AVCODEC
#include "DTFFHeader.h"

#include <openmedia/DTAdditionalSettings.h>
#include <openmedia/DTCodec.h>
#include <openmedia/DTDecoder.h>

namespace openmedia {

namespace details {

class _AVCodecContextDestroy
{
public:
    void operator()(AVCodecContext * _CodecContext)
    {   
        if (NULL != _CodecContext)
        {
            if (_CodecContext->codec)
            {
                avcodec_close(_CodecContext);
            }

            av_freep(&_CodecContext->extradata);
            av_freep(&_CodecContext->inter_matrix);
            av_freep(&_CodecContext->intra_matrix);
            av_freep(&_CodecContext->rc_eq);
            av_freep(&_CodecContext->rc_override);

           // avcodec_default_free_buffers(_CodecContext);
            av_free(_CodecContext);
        }
    }
};

} // namespace details

AVCodecContextPtr dt_create_codec_context()
{
    AVCodecContextPtr codecContext(avcodec_alloc_context(), details::_AVCodecContextDestroy());
    return codecContext;
}

AVCodecContextPtr dt_create_codec_context(AVCodecContext * _AVCodecContext)
{
    return AVCodecContextPtr(_AVCodecContext, details::_AVCodecContextDestroy());
}


void dt_open_ff_decoder(AVCodecContext * _AVCodecContext, dt_codec_id_t _CodecID)
{
    AVCodec * avDecoder = avcodec_find_decoder(DT2FFType(_CodecID));
    if (!avDecoder)
    {
        BOOST_THROW_EXCEPTION(decoder::create_error() << boost::errinfo_api_function("avcodec_find_decoder"));
    }

    int dt_err = avcodec_open(_AVCodecContext, avDecoder);
    FF_CHECK_ERR(dt_err);
    if (FF_ERR(dt_err))
		DT_THROW_AVERROR(decoder::create_error(), dt_err, "avcodec_open");
}

bool get_ff_codec_name_by_id(dt_codec_id_t _CodecId, std::string & _Name, std::string & _LongName)
{
    AVCodec * codec = NULL;
    while ((codec = av_codec_next(codec)))
    {
        if (codec->id == DT2FFType(_CodecId))
        {
            _Name = codec->name;
            _LongName = codec->long_name;
            return true;
        }
    }
    return false;    
}

}


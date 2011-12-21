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


#define DT_FF_NEED_AVFORMAT
#include "DTFFHeader.h"
#include "DTFFMediaMuxer.h"

#include <exception>
#include <openmedia/DTCommon.h>
#include <openmedia/DTError.h>
#include <openmedia/DTMediaMuxer.h>
#include "../DTMediaMuxerImpl.h"

namespace openmedia {

namespace details {

class _AVOutputFormatContextDestruct
{
public:
    void operator()(AVFormatContext * _AVFormatContext)
    {
        if (_AVFormatContext)
        {
            if (!(_AVFormatContext->oformat->flags & AVFMT_NOFILE) && _AVFormatContext->pb)
                avio_close(_AVFormatContext->pb);
            avformat_free_context(_AVFormatContext);
        }
    }
};

} // namespace details


AVOutputFormat * dt_av_guess_format(const char * _ShortName, const char * _MimeType)
{
    AVOutputFormat * outputFormat = av_guess_format(_ShortName, NULL, _MimeType);
    return outputFormat;
}

AVFormatContextPtr dt_av_create_output_format_context(const char * _FormatName, const char * _Filename)
{
    AVOutputFormat * outputFormat = dt_av_guess_format(_FormatName, NULL);
    
    AVFormatContext * outputCtx = NULL;
    int dt_err = avformat_alloc_output_context2(&outputCtx, outputFormat, NULL, _Filename);
    FF_CHECK_ERR(dt_err);

    if (FF_ERR(dt_err))
    {
        //// TODO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1 Error
        DT_THROW_AVERROR( errors::create_file() , dt_err, "avformat_alloc_output_context2");
        return AVFormatContextPtr((AVFormatContext*)NULL);
    }

    AVFormatContextPtr outputCtxPtr( outputCtx, details::_AVOutputFormatContextDestruct() );

    if (!(outputCtx->oformat->flags & AVFMT_NOFILE)) 
    {
        dt_err = avio_open(&outputCtx->pb, _Filename, AVIO_FLAG_WRITE);
        if (FF_ERR(dt_err))
        {
            //// TODO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1 Error
            DT_THROW_AVERROR( errors::create_file() , dt_err, "avformat_alloc_output_context2");
            return AVFormatContextPtr((AVFormatContext*)NULL);
        }
    }

    return outputCtxPtr;
}

class ff_simple_media_muxer_impl : public media_muxer::Impl
{
public:
    ff_simple_media_muxer_impl(const char * _FormatName, const char * _Filename);

public:
    virtual void write_packet(media_packet_ptr _MediaPacket);
    virtual void close();

private:

    AVFormatContextPtr outputCtx_;
};

ff_simple_media_muxer_impl::ff_simple_media_muxer_impl(const char * _FormatName, const char * _Filename) :
outputCtx_( dt_av_create_output_format_context(_FormatName, _Filename) )
{
}

void ff_simple_media_muxer_impl::write_packet(media_packet_ptr _MediaPacket)
{

}

void ff_simple_media_muxer_impl::close()
{

}

//////

ff_simple_media_muxer::ff_simple_media_muxer(const char * _FormatName, const char * _Filename) :
media_muxer( new ff_simple_media_muxer_impl(_FormatName, _Filename) )
{
}

}

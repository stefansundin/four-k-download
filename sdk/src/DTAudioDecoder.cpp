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

/// \file   DTAudioDecoder.cpp

#include <openmedia/DTAudioDecoder.h>
#include <openmedia/DTAudioDecoderInfo.h>

#include "DTAudioDecoderImpl.h"
#include "ffmpeg/DTFFAudioDecoder.h"

namespace openmedia {

std::string audio_decoder_additional_info::get_codec_name() const
{
    return impl()->get_codec_name();
}

std::string audio_decoder_additional_info::get_codec_long_name() const
{
    return impl()->get_codec_long_name();
}

audio_decoder_additional_info::~audio_decoder_additional_info()
{
    delete m_pImpl;
}

audio_decoder_additional_info::audio_decoder_additional_info(Impl * _Impl) : m_pImpl(_Impl)
{
    DT_STRONG_ASSERT( NULL != _Impl );
}

audio_data_ptr audio_decoder::decode(media_packet_ptr _MediaPacket)
{
    return impl()->decode(_MediaPacket);
}

audio_data_timed_ptr audio_decoder::decode_timed(media_packet_ptr _MediaPacket)
{
    return impl()->decode_timed(_MediaPacket);
}

audio_decoder_info_ptr  audio_decoder::get_decoder_info() const
{
    return impl()->get_decoder_info();
}

audio_decoder_additional_info_ptr audio_decoder::get_additional_info() const
{
    return impl()->get_additional_info();
}

audio_decoder::audio_decoder(audio_decoder::Impl * _Impl):decoder(_Impl), m_pImpl(_Impl)
{
    DT_ASSERT(NULL != _Impl);
}

audio_decoder_ptr audio_decoder::create(const audio_decoder_info * _DecoderInfo)
{    
    switch (_DecoderInfo->get_codec_base())
    {
    case CODEC_BASE_FFMPEG:
        return audio_decoder_ptr( new ff_audio_decoder(_DecoderInfo) );
        break;
    default:
        DT_ASSERT(false);
        return audio_decoder_ptr((audio_decoder*)NULL);
        break;
    }
}

} //namespace openmedia 

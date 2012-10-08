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

/// \file   DTAudioDecoderInfo.cpp

#include <openmedia/DTAudioDecoderInfo.h>
#include "DTAudioDecoderInfoImpl.h"
#include "DTAudioDecoderInfoCommon.h"

namespace openmedia {

int audio_decoder_info::get_sample_rate() const
{
    return impl()->get_sample_rate();
}

int audio_decoder_info::get_channels_count() const
{
    return impl()->get_channels_count();
}

dt_sample_format_t audio_decoder_info::get_sample_format() const
{
    return impl()->get_sample_format();
}

int audio_decoder_info::get_sample_align() const
{
    return impl()->get_sample_align();
}

dt_channel_layout_t audio_decoder_info::get_channel_layout() const
{
    return impl()->get_channel_layout();
}

int audio_decoder_info::get_bits_per_coded_sample() const
{
    return impl()->get_bits_per_coded_sample();
}

int audio_decoder_info::get_bits_per_raw_sample() const
{
    return impl()->get_bits_per_raw_sample();
}

///////////////////////////////////////

audio_decoder_info::audio_decoder_info(audio_decoder_info::Impl * _Impl): decoder_info(_Impl), m_pImpl(_Impl)
{
}

audio_decoder_info_ptr audio_decoder_info::create(dt_codec_id_t _CodecID, 
        int _SampleRate,
        int _ChannelsCount,
        const uint8_t * _Extradata, 
        int _ExtradataSize)
{
    return audio_decoder_info_ptr( new audio_decoder_info( new audio_decoder_info_simple(_CodecID, _SampleRate, _ChannelsCount, _Extradata, _ExtradataSize) ) );
}

}

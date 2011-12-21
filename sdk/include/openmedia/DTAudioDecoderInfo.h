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


/// \file   DTAudioDecoderInfo.h

#ifndef _DTAUDIODECODERINFO_H_INCLUDED_
#define _DTAUDIODECODERINFO_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <openmedia/DTDecoderInfo.h>
#include <openmedia/DTSampleFormatTypes.h>

namespace openmedia {

class audio_decoder_info;
typedef boost::shared_ptr<audio_decoder_info> audio_decoder_info_ptr;

/// \class  audio_decoder_info
class _OPENMEDIASDK_API audio_decoder_info: public decoder_info
{
    DT_DECLARE_IMPL2(audio_decoder_info);
public:
    int                 get_sample_rate() const;
    int                 get_channels_count() const;
    dt_sample_format_t  get_sample_format() const;
    int                 get_sample_align() const;
    dt_channel_layout_t get_channel_layout() const;
    int                 get_bits_per_coded_sample() const;
    int                 get_bits_per_raw_sample() const;

public:
    static audio_decoder_info_ptr create(dt_codec_id_t _CodecID, 
        int _SampleRate,
        int _ChannelsCount,
        const uint8_t * _Extradata, 
        int _ExtradataSize);

protected:
    audio_decoder_info(audio_decoder_info::Impl * _Impl);

private:
    audio_decoder_info::Impl * m_pImpl;

};

}

#endif

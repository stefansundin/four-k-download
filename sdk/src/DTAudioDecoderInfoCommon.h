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


/// \file   DTAudioDecoderInfoCommon.h

#ifndef _DTAUDIODDECODERINFOCOMMON_H_INCLUDED_
#define _DTAUDIODDECODERINFOCOMMON_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <openmedia/DTAudioDecoderInfo.h>
#include "DTAudioDecoderInfoImpl.h"
#include <vector>
#include <cstring>

namespace openmedia {

class audio_decoder_info_simple : public audio_decoder_info::Impl
{
private:
    dt_media_type_t mediaType_;
    dt_codec_base_t codecBase_;
    dt_codec_id_t codecID_;

    int sampleRate_;
    int channelsCount_;

    std::vector<uint8_t> extradata_;

public:
    audio_decoder_info_simple(dt_codec_id_t _CodecID, 
        int _SampleRate,
        int _ChannelsCount,
        const uint8_t * _Extradata, 
        int _ExtradataSize) 
        : 
      mediaType_(DT_AVMEDIA_TYPE_AUDIO),
      codecBase_(CODEC_BASE_FFMPEG),
      codecID_(_CodecID),
      sampleRate_(_SampleRate),
      channelsCount_(_ChannelsCount),
      extradata_(_ExtradataSize)
    {
        if (_ExtradataSize > 0)
            memcpy(&(extradata_[0]), _Extradata, _ExtradataSize);
    }    

public:
    virtual const uint8_t * get_extradata() const { return (extradata_.size() ? &(extradata_[0]) : NULL ); }
    virtual int             get_extradata_size() const { return extradata_.size(); }
    virtual std::string     get_codec_name() const { return ""; }
    virtual dt_codec_base_t get_codec_base() const { return codecBase_; }
    virtual dt_media_type_t get_codec_type() const { return mediaType_; }
    virtual dt_codec_id_t   get_codec_id() const { return codecID_; }
    virtual unsigned int    get_codec_tag() const { return 0; }
    virtual int             get_profile() const { return 0; }
    virtual int             get_level() const { return 0; }
    virtual int             get_bit_rate() const { return 0; }
    virtual dt_rational_t   get_time_base() const { return dt_rational_t(); }
    virtual int             get_rc_max_rate() const { return 0; }
    virtual int             get_rc_buffer_size() const { return 0; } 

public:
    virtual int                 get_sample_rate() const { return sampleRate_; } 
    virtual int                 get_channels_count() const { return channelsCount_; }
    virtual dt_sample_format_t  get_sample_format() const { return DT_SAMPLE_FMT_S16; }
    virtual int                 get_block_align() const { return 0; }
    virtual dt_channel_layout_t get_channel_layout() const { return 0; }
    virtual int                 get_bits_per_coded_sample() const { return 0; }
    virtual int                 get_bits_per_raw_sample() const { return 0; }

};

}

#endif // #ifndef _DTAUDIODDECODERINFOCOMMON_H_INCLUDED_

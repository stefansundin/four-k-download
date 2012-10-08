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


/// \file   DTAudioFormat.h

#ifndef _DTAUDIOFORMAT_H_INCLUDED_
#define _DTAUDIOFORMAT_H_INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <openmedia/DTConfig.h>
#include <openmedia/DTDeclareImpl.h>
#include <openmedia/DTTypes.h>
#include <openmedia/DTSampleFormatTypes.h>
#include <openmedia/DTCommon.h>
#include <openmedia/DTError.h>

namespace openmedia {

class audio_format;
typedef boost::shared_ptr<audio_format> audio_format_ptr;

class media_settings;
typedef boost::shared_ptr<media_settings> media_settings_ptr;

class audio_decoder_info;

/// \class  audio_format
class _OPENMEDIASDK_API audio_format: interface_base
{
    DT_DECLARE_IMPL2(audio_format);

public:
    struct invalid_format: public virtual errors::dt_error
    {
    public:
        invalid_format();    
    };

public:
    static const char *optSampleRate, *optSampleFormat, *optChannelsCount, *optChannelLayout;

public:
	int					get_sample_rate() const;
    dt_sample_format_t  get_sample_format() const;
    dt_channel_layout_t get_channel_layout() const;
    int                 get_channels_count() const;
    int                 get_bits_per_sample() const;
    ptrdiff_t           get_sample_align() const;
    audio_format_ptr    duplicate() const;
    audio_format_ptr    clone() const;
    
    virtual             ~audio_format();

public:
    static audio_format_ptr create(media_settings_ptr settings);
    static media_settings_ptr serialize(const audio_format * audioFormat);

    static audio_format_ptr create(int _SampleRate, dt_sample_format_t _SampleFormat, int _ChannelsCount);
    static audio_format_ptr create(int _SampleRate, dt_sample_format_t _SampleFormat, int _ChannelsCount, dt_channel_layout_t _ChannelLayout);
    static audio_format_ptr create(const audio_format * _AudioFormat);
    static audio_format_ptr create(const audio_decoder_info * audioDecoderInfo);
    static size_t size_bytes(const audio_format * _AudioFormat, size_t _SamplesCount);

protected:
    audio_format(audio_format::Impl * _Impl);
    
private:
    audio_format::Impl * m_pImpl;

};

};

#endif

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


/// \file   DTAudioData.h

#ifndef _DTAUDIODATA_H_INCLUDED_
#define _DTAUDIODATA_H_INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <openmedia/DTDeclareImpl.h>
#include <openmedia/DTCommon.h>
#include <openmedia/DTCStdInt.h>
#include <openmedia/DTTypes.h>
#include <openmedia/DTSampleFormatTypes.h>

namespace openmedia {

class audio_format;
class audio_data;
typedef boost::shared_ptr<audio_data> audio_data_ptr;

class media_packet;
typedef boost::shared_ptr<media_packet> media_packet_ptr;

/// \class  audio_data
class _OPENMEDIASDK_API audio_data: interface_base
{
    DT_DECLARE_IMPL2(audio_data);
public:
    const uint8_t *     get_raw_data() const;
    const uint8_t *     get_data_at(int _ChannelNum, int _SampleNum) const;
    dt_data_size_t      get_raw_size() const;
    ptrdiff_t           get_sample_align() const;
    int                 get_samples_count() const;
    dt_sample_format_t  get_sample_format() const;
	dt_channel_layout_t get_channel_layout() const;
    int                 get_channels_count() const;
    int                 get_bits_per_sample() const;
    uint8_t *           lock_data();
    void                unlock_data();
    virtual             ~audio_data() = 0;

public:
    static audio_data_ptr create_sin(const audio_format * _AudioFormat, double _DurationMS, double _Frequency, double _Phase);
    static audio_data_ptr create_silence(const audio_format * _AudioFormat, double _DurationMS);
    static media_packet_ptr to_packet(audio_data_ptr _AudioData);
        
protected:
    audio_data(audio_data::Impl * _Impl);
	audio_data();
	void reset_impl(audio_data::Impl * _Impl);
    
private:
    audio_data::Impl * m_pImpl;

};

} // namespace openmedia 

#endif // #ifndef _DTAUDIODATA_H_INCLUDED_

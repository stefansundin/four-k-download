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


/// \file   DTAudioDataImpl.h

#ifndef _DTAUDIODATAIMPL_H_INCLUDED_
#define _DTAUDIODATAIMPL_H_INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/noncopyable.hpp>

#include <openmedia/DTCommon.h>
#include <openmedia/DTCStdInt.h>

namespace openmedia {

/// \class  audio_data::Impl
class audio_data::Impl: impl_base
{
public:
    virtual const uint8_t *     get_raw_data() const = 0;
    virtual const uint8_t *     get_data_at(int _ChannelNum, int _SampleNum) const;
    virtual dt_data_size_t      get_raw_size() const = 0;
    virtual ptrdiff_t           get_sample_align() const = 0;
    virtual int                 get_samples_count() const = 0;
    virtual dt_sample_format_t  get_sample_format() const = 0;
	virtual dt_channel_layout_t get_channel_layout() const = 0;
    virtual int                 get_channels_count() const = 0;
    virtual int                 get_bits_per_sample() const = 0;
    virtual uint8_t *           lock_data() = 0;
    virtual void                unlock_data() = 0;

};

inline const uint8_t * audio_data::Impl::get_data_at(int _ChannelNum, int _SampleNum) const
{
    const uint8_t * data = get_raw_data();
    return data + get_sample_align() * _SampleNum + _ChannelNum * get_bits_per_sample();
}

} // namespace openmedia 

#endif

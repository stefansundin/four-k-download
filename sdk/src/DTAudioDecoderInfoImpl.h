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


/// \file   DTAudioDecoderInfoImpl.h

#ifndef _DTAUDIODECODERINFOIMPL_H_INCLUDED_
#define _DTAUDIODECODERINFOIMPL_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <openmedia/DTDecoderInfo.h>
#include "DTDecoderInfoImpl.h"

namespace openmedia {

/// \class  audio_decoder_info::Impl
class audio_decoder_info::Impl: public decoder_info::Impl
{
public:
    virtual int                 get_sample_rate() const = 0;
    virtual int                 get_channels_count() const = 0;
    virtual dt_sample_format_t  get_sample_format() const = 0;
    virtual int                 get_block_align() const = 0;
    virtual dt_channel_layout_t get_channel_layout() const = 0;
    virtual int                 get_bits_per_coded_sample() const = 0;
    virtual int                 get_bits_per_raw_sample() const = 0;
    int                         get_sample_align() const;

};

inline int audio_decoder_info::Impl::get_sample_align() const
{
    return get_block_align();
}

}

#endif

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


/// \file   DTAudioEncoderImpl.h

#ifndef _DTAUDIOENCODERIMPL_H_INCLUDED_
#define _DTAUDIOENCODERIMPL_H_INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <openmedia/DTCommon.h>
#include <openmedia/DTAudioEncoder.h>
#include <openmedia/DTPacket.h>
#include <openmedia/DTAudioData.h>

namespace openmedia {

class audio_encoder::Impl : impl_base
{
public:
    virtual media_packet_ptr        encode(const audio_data * _AudioData) = 0;  
    virtual codec_extra_data_ptr    get_extra_data() const = 0;
};

} // namespace openmedia

#endif // #ifndef _DTAUDIOENCODERIMPL_H_INCLUDED_

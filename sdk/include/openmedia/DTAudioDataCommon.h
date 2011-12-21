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


/// \file   DTAudioDataCommon.h

#ifndef _DTAUDIODATACOMMON_H_INCLUDED_
#define _DTAUDIODATACOMMON_H_INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/shared_ptr.hpp>

#include <openmedia/DTCommon.h>
#include <openmedia/DTAudioData.h>

namespace openmedia {

class audio_format;
class audio_decoder_info;

class audio_data_common;
typedef boost::shared_ptr<audio_data_common> av_audio_data_ptr;

class _OPENMEDIASDK_API audio_data_common: public audio_data
{
public:
    enum OwnBuffer 
    {
        bufferRef,
        bufferGetOwn,
        bufferAllocNew
    };

public:
    audio_data_common();
    audio_data_common(const audio_decoder_info * _AudioDecoderInfo);
    audio_data_common(const audio_format * _AudioFormat, uint8_t * _RawBuffer, size_t _SizeBytes, OwnBuffer _OwnBuffer);

public:
    static uint8_t * alloc_buffer(size_t _SizeBytes);
    static void free_buffer(uint8_t * _Buffer);
        
};

} // namespace openmedia 

#endif // #ifndef _DTAUDIODATACOMMON_H_INCLUDED_

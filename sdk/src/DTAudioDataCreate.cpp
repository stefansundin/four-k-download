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


/// \file   DTAudioDataCreate.cpp

#include "DTHeadersMedia.h"

#include <openmedia/DTAudioData.h>
#include <openmedia/DTAudioFormat.h>
#include <openmedia/DTAudioUtils.h>

#include <cstring>
#include <limits>

#include "DTAudioDataImpl.h"
#include <openmedia/DTAudioDataCommon.h>

namespace openmedia {

namespace details {
void create_buffer(const audio_format * _AudioFormat, double _DurationMS, uint8_t * & _OutBuffer, size_t & _OutSizeBytes, int & _OutSamplesCount)
{
    const double samplesCountDbl = _DurationMS / 1000.0 *_AudioFormat->get_sample_rate();
    DT_ASSERT( samplesCountDbl < (double)(std::numeric_limits<int>::max)() );
    // TODO?

    const int samplesCount = static_cast<int>(samplesCountDbl);
    const size_t sizeBytes = samplesCount * _AudioFormat->get_sample_align();
    uint8_t * buffer = audio_data_common::alloc_buffer(sizeBytes);
    
    _OutBuffer          = buffer;
    _OutSizeBytes       = sizeBytes;
    _OutSamplesCount    = samplesCount;
}
}

audio_data_ptr audio_data::create_sin(const audio_format * _AudioFormat, double _DurationMS, double _Frequency, double _Phase)
{
    uint8_t * buffer = NULL;
    size_t bufferSizeBytes = 0;
    int samplesCount = 0;
    details::create_buffer(_AudioFormat, _DurationMS, buffer, bufferSizeBytes, samplesCount);
    
    // TODO

    return audio_data_ptr( new audio_data_common(_AudioFormat, buffer, bufferSizeBytes, audio_data_common::bufferGetOwn ) );
}

audio_data_ptr audio_data::create_silence(const audio_format * _AudioFormat, double _DurationMS)
{
    uint8_t * buffer = NULL;
    size_t bufferSizeBytes = 0;
    int samplesCount = 0;
    details::create_buffer(_AudioFormat, _DurationMS, buffer, bufferSizeBytes, samplesCount);

    // TODO : 
    memset(buffer, utils::audio::silence_value(_AudioFormat->get_sample_format()), bufferSizeBytes);
    return audio_data_ptr( new audio_data_common(_AudioFormat, buffer, bufferSizeBytes, audio_data_common::bufferGetOwn ) );
}

} // namespace openmedia

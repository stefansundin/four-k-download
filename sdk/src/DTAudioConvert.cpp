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

/// \file   DTAudioConvert.cpp

#include <openmedia/DTAudioConvert.h>
#include "DTAudioBufferImpl.h"
#include "ffmpeg/DTFFSWResample.h"

namespace openmedia {

audio_convert::audio_convert(audio_buffer::Impl * _Impl) : audio_buffer(_Impl)
{
}

audio_data_ptr audio_convert::convert(const audio_data * _AudioData)
{
    impl()->push_back(_AudioData);
    int availSamplesCount = impl()->get_samples_count();
    return impl()->pop_front(availSamplesCount);
}

//audio_convert_ptr audio_convert_utils::create_sample_fmt_convert(const audio_format * _OutputAudioFormat)
//{
//    return audio_convert_ptr( new ff_sample_fmt_converter(_OutputAudioFormat) );
//}
//
//audio_convert_ptr audio_convert_utils::create_resample_convert(const audio_format * _OutputAudioFormat, int _InputSampleRate, int _OutputSampleRate)
//{
//    return audio_convert_ptr( new ff_resample_converter(_OutputAudioFormat, _InputSampleRate, _OutputSampleRate) );
//}

audio_convert_ptr audio_convert_utils::create_resample_convert(const audio_format * _OutputAudioFormat, const audio_format * _InputAudioFormat)
{
    return audio_convert_ptr( new ff_swresample(_InputAudioFormat, _OutputAudioFormat) );
}

}
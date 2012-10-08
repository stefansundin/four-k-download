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


/// \file   DTAudioConvert.h

#ifndef _DTAUDIOCONVERT_H_INCLUDED_D2437194
#define _DTAUDIOCONVERT_H_INCLUDED_D2437194

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <openmedia/DTDeclareImpl.h>
#include <openmedia/DTConfig.h>
#include <openmedia/DTCommon.h>
#include <openmedia/DTAudioBuffer.h>

namespace openmedia {

class audio_data;
typedef boost::shared_ptr<audio_data> audio_data_ptr;

class audio_format;
typedef boost::shared_ptr<audio_format> audio_format_ptr;

class audio_convert;
typedef boost::shared_ptr<audio_convert> audio_convert_ptr;

/// \class  audio_convert
class _OPENMEDIASDK_API audio_convert: public audio_buffer
{
public:
    audio_data_ptr convert(const audio_data * audioData);

protected:
    audio_convert(audio_buffer::Impl * _Impl);  
};

class _OPENMEDIASDK_API audio_convert_utils
{
public:
    // create sample format converter
    // input and ouptut channels layout must be same
    //static audio_convert_ptr create_sample_fmt_convert(const audio_format * _OutputAudioFormat);
    //static audio_convert_ptr create_resample_convert(const audio_format * _OutputAudioFormat, int _InputSampleRate, int _OutputSampleRate);
    static audio_convert_ptr create_resample_convert(const audio_format * out, const audio_format * in);
};


} // namespace openmedia

#endif

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


/// \file   DTFFResampleConvert.h

#ifndef _DTFFRESAMPLECONVERT_H_INCLUDED_
#define _DTFFRESAMPLECONVERT_H_INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <openmedia/DTCommon.h>
#include <openmedia/DTAudioBuffer.h>
#include <openmedia/DTAudioConvert.h>
#include "../DTAudioBufferImpl.h"

namespace openmedia {

/// \class  ff_resample_converter
class ff_resample_converter : public audio_convert
{
public:
    class properties;

public:
    ff_resample_converter(const audio_format * _OutputAudioFormat, int _InputSampleRate, int _OutputSampleRate);
    ff_resample_converter(const audio_format * _OutputAudioFormat, int _InputSampleRate, int _OutputSampleRate, const properties * _Properties);
};

} // namespace openmedia

#endif // #ifndef _DTFFRESAMPLECONVERT_H_INCLUDED_

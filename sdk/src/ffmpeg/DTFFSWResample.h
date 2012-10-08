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


/// \file   DTFFSWResample.h

#ifndef _DTFFSWRESAMPLE_H_INCLUDED_
#define _DTFFSWRESAMPLE_H_INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <openmedia/DTCommon.h>
#include <openmedia/DTAudioBuffer.h>
#include <openmedia/DTAudioConvert.h>
#include <openmedia/DTError.h>

namespace openmedia {

class ff_swresample : public audio_convert
{
public:
    class error : public virtual errors::dt_error {};
public:
    ff_swresample(const audio_format * _InputAudioFormat, const audio_format * _OutputAudioFormat);
};

} // namespace openmedia

#endif

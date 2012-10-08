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


/// \file   DTAudioBufferImpl.h

#ifndef _DTAUDIOBUFFERIMPL_H_INCLUDED_
#define _DTAUDIOBUFFERIMPL_H_INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <openmedia/DTDeclareImpl.h>
#include <openmedia/DTCommon.h>
#include <openmedia/DTAudioBuffer.h>
#include <openmedia/DTAudioData.h>
#include <openmedia/DTAudioFormat.h>

namespace openmedia {

/// \class  audio_buffer::Impl
class audio_buffer::Impl : public impl_base
{
public:
    virtual void                push_back(const audio_data * audioData) = 0;
    virtual audio_data_ptr      pop_front(int samplesCount) = 0; 
    virtual void                push_back_raw(const uint8_t * data, size_t dataSize) = 0;
    virtual audio_format_ptr    get_output_audio_format() const = 0;
    virtual int                 get_samples_count() const = 0;

};

} // namespace openmedia

#endif // #ifndef _DTAUDIOBUFFERIMPL_H_INCLUDED_

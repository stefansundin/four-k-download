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


/// \file   DTAudioBuffer.h

#ifndef OPENMEDIA_DTAUDIOBUFFER_H_INCLUDED_AAB8E78A
#define OPENMEDIA_DTAUDIOBUFFER_H_INCLUDED_AAB8E78A

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <openmedia/DTDeclareImpl.h>
#include <openmedia/DTCStdInt.h>
#include <openmedia/DTConfig.h>
#include <openmedia/DTCommon.h>
#include <openmedia/DTError.h>

namespace openmedia {

class audio_data;
typedef boost::shared_ptr<audio_data> audio_data_ptr;

class audio_format;
typedef boost::shared_ptr<audio_format> audio_format_ptr;

class   audio_buffer;
typedef boost::shared_ptr<audio_buffer> audio_buffer_ptr;

/// \class  audio_buffer
class _OPENMEDIASDK_API audio_buffer: interface_base
{
    DT_DECLARE_IMPL2(audio_buffer);

public:
    enum AudioBufferType
    {
        audio_buffer_SameFormat = 0
    };

    static audio_buffer_ptr create(AudioBufferType bufferType, const audio_format * audioFormat);

    struct invalid_format: public virtual errors::dt_error
    {
    public:
        invalid_format();    
    };

public:
    void                push_back(const audio_data * audioData); 
    audio_data_ptr      pop_front(int samplesCount);
    void                push_back_raw(const uint8_t * data, size_t dataSize); 
    audio_format_ptr    get_output_audio_format() const;
    int                 get_samples_count() const;
    virtual             ~audio_buffer();
            
protected:
    audio_buffer(Impl * _Impl);    

private:
    Impl * m_pImpl;
};

}

#endif

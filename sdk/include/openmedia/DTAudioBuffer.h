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

#ifndef _DTAUDIOBUFFER_H_INCLUDED_
#define _DTAUDIOBUFFER_H_INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <openmedia/DTDeclareImpl.h>
#include <openmedia/DTConfig.h>
#include <openmedia/DTCommon.h>
#include <openmedia/DTAudioData.h>
#include <openmedia/DTAudioFormat.h>
#include <openmedia/DTError.h>

namespace openmedia {

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

    static audio_buffer_ptr create(AudioBufferType _BufferType, const audio_format * _AudioFormat);

    struct invalid_format: public virtual errors::dt_error
    {
    public:
        invalid_format();    
    };

public:
    void                push_back(const audio_data * _AudioData); 
    audio_data_ptr      pop_front(int _SamplesCount);
    void                push_back_raw(const uint8_t * _Data, size_t _DataSize); 
    audio_format_ptr    get_output_audio_format() const;
    int                 get_samples_count() const;

    virtual             ~audio_buffer();
            
protected:
    audio_buffer(audio_buffer::Impl * _Impl);    

private:
    audio_buffer::Impl * m_pImpl;
};

}

#endif

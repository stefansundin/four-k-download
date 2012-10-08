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


/// \file   DTAudioEncoderQueue.h

#ifndef _DTAUDIOENCODERQUEUE_H_INCLUDED_
#define _DTAUDIOENCODERQUEUE_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <openmedia/DTCommon.h>
#include <openmedia/DTDeclareImpl.h>
#include <openmedia/DTError.h>

namespace openmedia {

class audio_data;

template <class MediaData> class media_data_timed;
typedef media_data_timed<audio_data> audio_data_timed;
typedef boost::shared_ptr<audio_data_timed> audio_data_timed_ptr;

class media_packet;
typedef boost::shared_ptr<media_packet> media_packet_ptr;

class codec_extra_data;
typedef boost::shared_ptr<codec_extra_data> codec_extra_data_ptr;

class audio_encoder_queue;
typedef boost::shared_ptr<audio_encoder_queue> audio_encoder_queue_ptr;

/// \class  audio_encoder_queue
class _OPENMEDIASDK_API audio_encoder_queue : interface_base 
{
    DT_DECLARE_IMPL2(audio_encoder_queue);
public:
    struct error : virtual errors::dt_error 
    {
        error(const std::string & msg = "failed"): dt_error((std::string("audio encoder: ") + msg).c_str())
        {}
    };

public:
    void                    open();
    void                    send_audio(const audio_data * audioData);
    void                    send_audio(const audio_data_timed * audioDataTimed);
    void                    flush();
    media_packet_ptr        receive_packet();    

    codec_extra_data_ptr    get_extra_data() const;
    void *                  get_private_data() const;
    virtual ~audio_encoder_queue();
    
protected:
    audio_encoder_queue(Impl * _Impl);

private:
    Impl * m_pImpl;

};

} // namespace openmedia

#endif // #ifndef _DTAUDIOENCODERQUEUE_H_INCLUDED_

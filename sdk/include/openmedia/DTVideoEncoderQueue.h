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


/// \file   DTVideoEncoderQueue.h

#ifndef _DTVIDEOENCODERQUEUE_H_INCLUDED_
#define _DTVIDEOENCODERQUEUE_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/shared_ptr.hpp>

#include <openmedia/DTConfig.h>
#include <openmedia/DTCommon.h>
#include <openmedia/DTCStdInt.h>
#include <openmedia/DTError.h>

#include <openmedia/DTDeclareImpl.h>
#include <openmedia/DTTiming.h>

namespace openmedia {

class media_packet;
typedef boost::shared_ptr<media_packet> media_packet_ptr;

class codec_extra_data;
typedef boost::shared_ptr<codec_extra_data> codec_extra_data_ptr;

class video_data;
typedef media_data_timed<video_data> video_data_timed;

class video_encoder_queue;
typedef boost::shared_ptr<video_encoder_queue> video_encoder_queue_ptr;

class _OPENMEDIASDK_API video_encoder_queue : interface_base 
{
    DT_DECLARE_IMPL2(video_encoder_queue);
public:
    struct error : virtual errors::dt_error     
    {
        error(const std::string & msg = "failed"): dt_error((std::string("video encoder: ") + msg).c_str())
        {}
    };

public:
    void                    open();
    void                    send_video(const video_data * _VideoData);
    void                    send_video(const video_data_timed * _VideoDataTimed);
    media_packet_ptr        receive_packet(); 

    void * get_private_data() const;

protected:
    video_encoder_queue(video_encoder_queue::Impl * _Impl);

private:
    Impl * m_pImpl;

};

}

#endif // #ifndef _DTVIDEOENCODERQUEUE_H_INCLUDED_

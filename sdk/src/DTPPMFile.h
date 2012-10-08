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


/// \file   DTPPMFile.h

#ifndef _DTPPMFILE_H_INCLUDED_
#define _DTPPMFILE_H_INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <openmedia/DTMediaMuxer.h>
#include <openmedia/DTVideoEncoder.h>
#include "DTVideoEncoderFactory.h"
#include <vector>
#include <queue>

namespace openmedia {

class ppm_encoder_impl : public video_encoder_queue::Impl
{
public:
    ppm_encoder_impl();
    ppm_encoder_impl(media_settings_ptr);

public:
    virtual void                open() {}
    virtual void                send_video(const video_data * _VideoData);
    virtual media_packet_ptr    receive_packet();    

private:
    unsigned char * get_buffer(size_t _Size)
    {
        if (buffer_.size() < _Size)
            buffer_.resize(_Size);            

        return & buffer_[0];
    }

    std::vector<unsigned char> buffer_;
    std::queue<media_packet_ptr> packetQueue_;

private:
    int format;
    int customWidth, customHeight, customLinesize;

};

REGISTER_VIDEO_ENCODER(ppm_encoder_impl, "ppm");

}

#endif

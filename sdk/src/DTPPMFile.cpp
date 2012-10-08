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


/// \file   DTPPMFile.cpp
#include "DTHeadersMedia.h"

#include "DTPPMFile.h"

#include <sstream>
#include <vector>
#include <queue>

#include <boost/make_shared.hpp>

#include <openmedia/DTError.h>
#include <openmedia/DTPacket.h>
#include <openmedia/DTVideoData.h>

#include "DTVideoEncoderQueueImpl.h"
#include "DTCommonPacket.h"
#include <openmedia/DTMediaSettings.h>

using namespace std;
using namespace boost;

namespace openmedia {

namespace {
const unsigned int PPMMagickP6 = 6; // Portable pixmap
}

ppm_encoder_impl::
ppm_encoder_impl(): format(PPMMagickP6),
customWidth(0),
customHeight(0),
customLinesize(0)
{
}

ppm_encoder_impl::
ppm_encoder_impl(media_settings_ptr settings): format(6),
customWidth(0),
customHeight(0),
customLinesize(0)
{
    if (!settings)
        return;

    format = settings->get_param_or<int>("format", PPMMagickP6);
    customWidth = settings->get_param_or<int>("width", 0);
    customHeight = settings->get_param_or<int>("height", 0);
    customLinesize = settings->get_param_or<int>("line_size", 0);
}

void ppm_encoder_impl::send_video(const video_data * _VideoData)
{
    const int width         = customWidth ? customWidth : _VideoData->get_width();
    const int height        = customHeight ? customHeight : _VideoData->get_height();
    const uint8_t * data    = _VideoData->get_data();
    const int lineSize      = customLinesize ? customLinesize : _VideoData->get_line_size(0);

    ostringstream s;
    s << "P" << format << "\n" << width << " " << height << "\n255\n";
    
    const size_t bufferSize = s.str().size() + width * height * 3;
    unsigned char * buffer = get_buffer(bufferSize);
    memcpy(buffer, &(s.str()[0]), s.str().size());
    
    unsigned char * bitmap = buffer + s.str().size();

    for(int y = 0; y < height; ++y)
        memcpy(bitmap + 3*y*width, data + y*lineSize, 3*width);

    common_media_packet2_ptr mediaPacket = make_shared<common_media_packet2>();

    mediaPacket->setter()->set_data(buffer, bufferSize, bufferAllocNew);
    mediaPacket->setter()->set_media_type(DT_AVMEDIA_TYPE_VIDEO);

    packetQueue_.push( mediaPacket );
}

media_packet_ptr ppm_encoder_impl::receive_packet()
{
    if (!packetQueue_.empty())
    {
        media_packet_ptr outputPacket = packetQueue_.front();
        packetQueue_.pop();
        return outputPacket;
    }
    else
    {
        return media_packet_ptr();    
    }
}

/*
ppm_encoder::ppm_encoder() : video_encoder_queue( new ppm_encoder_impl() )
{
}
*/

}

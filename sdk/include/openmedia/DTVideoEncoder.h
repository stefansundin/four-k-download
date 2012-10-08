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


/// \file   DTVideoEncoder.h

#ifndef _DTVIDEOENCODER_H_INCLUDED_
#define _DTVIDEOENCODER_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

#include <openmedia/DTConfig.h>
#include <openmedia/DTCommon.h>

#include <openmedia/DTVideoEncoderQueue.h>

namespace openmedia {

class media_settings;
typedef boost::shared_ptr<media_settings> media_settings_ptr;

/// \class video_encoder_creator
class _OPENMEDIASDK_API video_encoder_creator
{
public:
    static video_encoder_queue_ptr create(const char * FormatName);
    static video_encoder_queue_ptr create(media_settings_ptr VideoEncoderSettings);
    static video_encoder_queue_ptr create(const char * FormatName, media_settings_ptr VideoEncoderSettings);
    static video_encoder_queue_ptr create(const std::string& FormatName, media_settings_ptr VideoEncoderSettings);
};



}

#endif // #ifndef _DTVIDEOENCODER_H_INCLUDED_

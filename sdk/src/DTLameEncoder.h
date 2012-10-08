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


/// \file   DTLameEncoder.h

#ifndef _DTLAMEENCODER_H_INCLUDED_
#define _DTLAMEENCODER_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <openmedia/DTAudioEncoder.h>
#include <openmedia/DTAudioData.h>
#include <openmedia/DTAudioFormat.h>

namespace openmedia {

class audio_encoder_lame_utils
{
public:
    class properties;
    typedef boost::shared_ptr<properties> properties_ptr;
    // TODO: default only
    static properties_ptr create_properties();
    static properties_ptr create_properties(const char * Artist, const char * Title, int Bitrate = -1);
};

class audio_encoder_lame : public audio_encoder
{
public:
    audio_encoder_lame(const audio_format * _InputAudioFormat, const audio_encoder_lame_utils::properties * _Properties);
    
};

class audio_encoder_queue_lame : public audio_encoder_queue
{
public:
    audio_encoder_queue_lame(const audio_format * _InputAudioFormat, const audio_encoder_lame_utils::properties * _Properties);
    
};

}

#endif

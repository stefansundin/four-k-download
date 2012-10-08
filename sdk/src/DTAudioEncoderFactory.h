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


#ifndef _OPENMEDIA_DT_AUDIO_ENCODER_FACTORY_H_894DDFD9
#define _OPENMEDIA_DT_AUDIO_ENCODER_FACTORY_H_894DDFD9

#include "DTEncoderFactory.h"
#include <openmedia/DTAudioEncoderQueue.h>
#include "DTAudioEncoderQueueImpl.h"

namespace openmedia {

class media_settings;

typedef encoder_factory<audio_encoder_queue, audio_encoder_queue::Impl, media_settings> audio_encoder_factory;

#define REGISTER_AUDIO_ENCODER(A, B) \
namespace { \
    register_encoder<A, audio_encoder_factory> register_audio_encoder##A(B);\
}


}

#endif

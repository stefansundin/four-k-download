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


#ifndef _OPENMEDIA_DT_AUDIO_FILTER_IMPL_H_08F13039
#define _OPENMEDIA_DT_AUDIO_FILTER_IMPL_H_08F13039

#include <openmedia/DTAudioFilters.h>
#include "DTAudioBufferImpl.h"


namespace openmedia {

class audio_filter::Impl : public audio_buffer::Impl
{
public:
    virtual void apply(media_settings * settings) = 0;
};

}

#endif

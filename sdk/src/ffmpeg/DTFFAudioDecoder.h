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


/// \file   DTFFAudioDecoder.h

#ifndef _DTFFAUDIODECODERIMPL_H_INCLUDED_
#define _DTFFAUDIODECODERIMPL_H_INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/noncopyable.hpp>

#include <openmedia/DTCommon.h>
#include <openmedia/DTAudioDecoder.h>

namespace openmedia {

class audio_decoder_info;

/// \class  ff_audio_decoder
class ff_audio_decoder: public audio_decoder
{
public:
    ff_audio_decoder(const audio_decoder_info * audioDecoderInfo);

};

}

#endif

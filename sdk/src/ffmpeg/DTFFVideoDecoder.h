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


/// \file   DTFFVideoDecoder.h

#ifndef _DTFFVIDEODECODER_H_INCLUDED_
#define _DTFFVIDEODECODERL_H_INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/noncopyable.hpp>

#include <openmedia/DTCommon.h>
#include <openmedia/DTVideoDecoder.h>

namespace openmedia {

class video_decoder_info;
class additional_settings;

/// \class  ff_video_decoder
class ff_video_decoder: public video_decoder
{
public:
    ff_video_decoder(video_decoder_info * _VideoDecoderInfo);
    ff_video_decoder(video_decoder_info * _VideoDecoderInfo, const additional_settings * _AdditionalSettings);

};

} // namespace openmedia

#endif // #ifndef _DTFFVIDEODECODER_H_INCLUDED_

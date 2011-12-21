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


/// \file   DTFFDecoderInfo.h

#ifndef _DTFFDECODERINFO_H_INCLUDED_
#define _DTFFDECODERINFO_H_INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/shared_ptr.hpp>

#include <openmedia/DTCommon.h>
#include <openmedia/DTDecoderInfo.h>
#include <openmedia/DTVideoDecoderInfo.h>
#include <openmedia/DTAudioDecoderInfo.h>

struct AVCodecContext;

namespace openmedia {

/// \class  ff_decoder_info
class ff_decoder_info: public decoder_info
{
public:
    ff_decoder_info(const AVCodecContext * _CodecContext);

    static decoder_info_ptr create(const AVCodecContext * _CodecContext)
    {
        return decoder_info_ptr(new ff_decoder_info(_CodecContext));
    }
};

/// \class  ff_video_decoder_info
class ff_video_decoder_info: public video_decoder_info
{
public:
    ff_video_decoder_info(const AVCodecContext * _CodecContext);

    static decoder_info_ptr create(const AVCodecContext * _CodecContext)
    {
        return decoder_info_ptr(new ff_video_decoder_info(_CodecContext));
    }
};

/// \class  ff_audio_decoder_info
class ff_audio_decoder_info: public audio_decoder_info
{
public:
    ff_audio_decoder_info(const AVCodecContext * _CodecContext);

    static decoder_info_ptr create(const AVCodecContext * _CodecContext)
    {
        return decoder_info_ptr(new ff_audio_decoder_info(_CodecContext));
    }
};

decoder_info_ptr create_ff_decoder_info(AVCodecContext * _Codec);

} // namespace openmedia

#endif

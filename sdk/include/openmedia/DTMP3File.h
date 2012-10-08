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


/// \file   DTMP3File.h

#ifndef _DTMP3FILE_H_INCLUDED_
#define _DTMP3FILE_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <openmedia/DTDeclareImpl.h>
#include <openmedia/DTCommon.h>
#include <openmedia/DTPacket.h>
#include <openmedia/DTMediaMuxer.h>
#include <openmedia/DTCodecExtraData.h>

namespace openmedia {

class audio_format;
class codec_extra_data;

class _OPENMEDIASDK_API media_muxer_mp3file : public media_muxer
{
public:
    media_muxer_mp3file(
        const char * _FileName,
        const audio_format * _AudioFormat,
        codec_extra_data_ptr _CodecExtraData
        );
};

} // namespace openmedia

#endif // #ifndef _DTMP3FILE_H_INCLUDED_

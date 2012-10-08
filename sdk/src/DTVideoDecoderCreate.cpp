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



// precompiled header begin
#include "DTHeadersMedia.h"
// precompiled header end

/// \file   DTVideoDecoderCreate.cpp

#include <openmedia/DTCommon.h>
#include <openmedia/DTVideoDecoder.h>
#include <openmedia/DTVideoDecoderInfo.h>
#include "DTVideoDecoderImpl.h"
#include "ffmpeg/DTFFVideoDecoder.h"

namespace openmedia {

video_decoder_ptr video_decoder_creator::create(video_decoder_info * _DecoderInfo)
{
    return create(_DecoderInfo, (const additional_settings *)NULL);
}

video_decoder_ptr video_decoder_creator::create(video_decoder_info * _DecoderInfo, const additional_settings * _AdditionalSettings)
{
    switch (_DecoderInfo->get_codec_base())
    {
    case CODEC_BASE_FFMPEG:
        return video_decoder_ptr(new ff_video_decoder(_DecoderInfo, _AdditionalSettings));
        break;
    default:
        DT_ASSERT(false);
        return video_decoder_ptr((video_decoder*)NULL);
        break;
    }
}

}

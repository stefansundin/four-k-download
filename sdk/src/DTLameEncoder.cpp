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

/// \file   DTLameEncoder.cpp

#include "DTLameEncoder.h"
#include "DTLameEncoderImpl.h"

namespace openmedia {

/*audio_encoder_lame::audio_encoder_lame(
                                       const audio_format * _InputAudioFormat,
                                       const audio_encoder_lame_utils::properties * _Properties
                                       ) :
audio_encoder( new audio_encoder_lame_impl(_InputAudioFormat, _Properties) )
{

}
*/

audio_encoder_queue_lame::audio_encoder_queue_lame(
    const audio_format * _InputAudioFormat,
    const audio_encoder_lame_utils::properties * _Properties):
audio_encoder_queue( new audio_encoder_queue_lame_impl(_InputAudioFormat, _Properties) )
{   

}

}

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

/// \file   DTVideoDecoder.cpp

#include <openmedia/DTCommon.h>
#include <openmedia/DTVideoDecoder.h>
#include "DTVideoDecoderImpl.h"

namespace openmedia {

video_data_ptr video_decoder::decode(media_packet_ptr _MediaPacket)
{
    return impl()->decode(_MediaPacket);
}

video_data_timed_ptr video_decoder::decode_timed(media_packet_ptr _MediaPacket)
{
    return impl()->decode_timed(_MediaPacket);
}

video_decoder_additional_info_ptr video_decoder::get_additional_info() const
{
    return impl()->get_additional_info();
}

video_decoder::video_decoder(video_decoder::Impl * _Impl):decoder(_Impl), m_pImpl(_Impl)
{
    DT_ASSERT(NULL != _Impl);
}

video_decoder_info_ptr video_decoder::get_decoder_info() const
{
    return impl()->get_decoder_info();
}

} //namespace openmedia 

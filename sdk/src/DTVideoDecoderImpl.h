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


/// \file   DTVideoDecoderImpl.h

#ifndef _DTVIDEODECODERIMPL_H_INCLUDED_
#define _DTVIDEODECODERIMPL_H_INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <openmedia/DTCommon.h>
#include <openmedia/DTPacket.h>
#include <openmedia/DTVideoData.h>
#include <openmedia/DTDecoderInfo.h>

#include "DTDecoderImpl.h"

namespace openmedia {

class media_packet;
typedef boost::shared_ptr<media_packet> media_packet_ptr;

/// \class  class video_decoder::Impl
class video_decoder::Impl: public decoder::Impl
{
public:
    virtual video_data_ptr          decode(media_packet_ptr _MediaPacket) = 0;
    virtual video_data_timed_ptr    decode_timed(media_packet_ptr _MediaPacket) = 0;
    virtual video_decoder_info_ptr  get_decoder_info() const = 0;
    virtual video_decoder_additional_info_ptr get_additional_info() const = 0;

};

} // namespace openmedia

#endif

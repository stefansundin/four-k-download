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


/// \file   DTFFPacket.h

#ifndef _DTFFPACKET_H_INCLUDED_
#define _DTFFPACKET_H_INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/shared_ptr.hpp>
#include <openmedia/DTCommon.h>
#include <openmedia/DTPacket.h>

struct AVPacket;

namespace openmedia {

typedef boost::shared_ptr<AVPacket> AVPacketPtr;
class ff_media_packet;
typedef boost::shared_ptr<ff_media_packet> av_media_packet_ptr;

/// \class  ff_media_packet
class ff_media_packet : public media_packet
{
public:
    ff_media_packet(AVPacketPtr _AVPacket);
    ff_media_packet(AVPacketPtr _AVPacket, dt_media_type_t _MediaType);
    ff_media_packet(AVPacketPtr _AVPacket, dt_media_type_t _MediaType, dt_rational_t _TimeBase);
    ff_media_packet(AVPacketPtr _AVPacket, dt_media_type_t _MediaType, dt_rational_t _TimeBase, dt_rational_t frameRate);
};

}

#endif // #ifndef _DTFFPACKET_H_INCLUDED_

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


/// \file  DTFFMediaMuxer.h

#ifndef _DTFFMEDIAMUXER_H_INCLUDED_
#define _DTFFMEDIAMUXER_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/shared_ptr.hpp>

#include <openmedia/DTCommon.h>
#include <openmedia/DTConfig.h>
#include <openmedia/DTMediaMuxer.h>

struct AVFormatContext;

namespace openmedia {

typedef boost::shared_ptr<AVFormatContext> AVFormatContextPtr;

class ff_simple_media_muxer : public media_muxer
{
public:
    ff_simple_media_muxer(const char * _FormatName, const char * _Filename);

};

}

#endif // #ifndef _DTFFMEDIAMUXER_H_INCLUDED_

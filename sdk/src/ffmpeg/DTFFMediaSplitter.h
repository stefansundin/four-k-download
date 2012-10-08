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


/// \file  DTAVMediaSplitter.h

#ifndef _DTAVMEDIASPLITTER_H_INCLUDED_
#define _DTAVMEDIASPLITTER_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/shared_ptr.hpp>

#include <openmedia/DTConfig.h>
#include <openmedia/DTMediaSplitter.h>

struct AVFormatContext;

namespace openmedia {

typedef boost::shared_ptr<AVFormatContext> AVFormatContextPtr;

/// \class  ff_media_splitter
class ff_media_splitter: public media_splitter
{
public:
    ff_media_splitter(AVFormatContextPtr _AVFormatContext);
    ff_media_splitter(const char * _Filename); 
};

} // namespace openmedia

#endif

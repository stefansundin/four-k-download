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


/// \file   DTVideoDataTimed.h

#ifndef _DTVIDEODATATIMED_H_INCLUDED_
#define _DTVIDEODATATIMED_H_INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/shared_ptr.hpp>

#include <openmedia/DTTiming.h>
#include <openmedia/DTVideoData.h>

namespace openmedia {

typedef media_data_timed<video_data> video_data_timed;
typedef boost::shared_ptr<video_data_timed> video_data_timed_ptr;

} // namespace openmedia

#endif // _DTVIDEODATATIMED_H_INCLUDED_

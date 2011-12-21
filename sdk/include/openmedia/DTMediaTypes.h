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


/// \file   DTSampleFormatTypes.h

#ifndef _DTMEDIATYPES_H_INCLUDED_
#define _DTMEDIATYPES_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace openmedia {

enum dt_media_type_t {
    DT_AVMEDIA_TYPE_UNKNOWN = -1,
    DT_AVMEDIA_TYPE_VIDEO,
    DT_AVMEDIA_TYPE_AUDIO,
    DT_AVMEDIA_TYPE_DATA,
    DT_AVMEDIA_TYPE_SUBTITLE,
    DT_AVMEDIA_TYPE_ATTACHMENT,
    DT_AVMEDIA_TYPE_NB
};

}

// #ifndef _DTMEDIATYPES_H_INCLUDED_
#endif

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


#ifndef _DT_SIMPLEMEDIAPARSER_H_
#define _DT_SIMPLEMEDIAPARSER_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <string>

namespace openmedia { namespace downloader {

bool flv_video_size(const std::string & content, int & width, int & height);
bool mp4_video_size(const std::string & content, int & width, int & height);
bool webm_video_size(const std::string & content, int & width, int & height);

} }


#endif

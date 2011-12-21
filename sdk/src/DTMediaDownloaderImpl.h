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


#ifndef _DT_MEDIADOWNLOADERIMPL_H_
#define _DT_MEDIADOWNLOADERIMPL_H_

#include <openmedia/DTMediaDownloader.h>

namespace openmedia { namespace downloader {

class media_downloader::Impl
{
public:
    virtual media_downloader::command_result_t pause() = 0;
    virtual media_downloader::command_result_t resume() = 0;
    virtual media_downloader::command_result_t cancel() = 0;

    virtual ~Impl() = 0;
};

inline media_downloader::Impl::~Impl()
{}

} }

#endif

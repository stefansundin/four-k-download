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


#ifndef _DT_SERVICESCRIPT_H_
#define _DT_SERVICESCRIPT_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <string>

#include <openmedia/DTMediaSite.h>
#include <boost/thread/mutex.hpp>

namespace openmedia { namespace downloader {

extern boost::mutex service_script_get_lock_;

boost::mutex & service_script_get_lock();

void set_service_script(media_site_type_t MediaSite, unsigned int Id, const std::string & Script);
void set_service_script_url(media_site_type_t MediaSite, unsigned int Id, const std::string & Url);

std::string get_service_script(media_site_type_t MediaSite, unsigned int Id);

} }

#endif // #ifndef _DT_SERVICESCRIPT_H_

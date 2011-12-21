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


#ifndef _DT_DOWNLOADERYOUTUBE_H_
#define _DT_DOWNLOADERYOUTUBE_H_

#include <openmedia/DTMediaDownloader.h>

#include <vector>

namespace openmedia { namespace downloader { namespace youtube {

media_download_list_ptr parse_url(const std::string & Url);
std::vector<media_download_list_ptr> parse_url_playlist(const std::string & Url);

} } } 

#endif //#ifndef _DT_DOWNLOADERYOUTUBE_H_

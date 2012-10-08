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


/// \file   DTMediaSite.h

#ifndef _DT_MEDIASITE_H_
#define _DT_MEDIASITE_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <string>

namespace openmedia { namespace downloader {

enum media_site_type_t
{
    mediaSiteNull = 0,
    mediaSiteUnknown = 1,
    mediaSiteYoutube = 2,
    mediaSiteVimeo = 4,
    mediaSiteDailymotion = 5,
    mediaSiteFacebook = 6,
    mediaSiteMetacafe = 8,
    mediaSiteRaiTv = 10
};

class media_site_utils
{
public:
    static media_site_type_t validate_url(const std::string & Url);
    static bool is_contain_playlist(const std::string & Url);
};

} }

#endif

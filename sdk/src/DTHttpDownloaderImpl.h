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


#ifndef _DT_HTTPDOWNLOADERIMPL_H_
#define _DT_HTTPDOWNLOADERIMPL_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <boost/function.hpp>

#include <openmedia/DTCommon.h>
#include <openmedia/DTHttpDownloader.h>

#include "DTHttpDownloaderInstance.h"

namespace openmedia { namespace downloader {

class http_downloader::Impl
{
public:
    http_downloader::command_result_t pause();
    http_downloader::command_result_t resume();
    http_downloader::command_result_t cancel();

public:
    Impl( http_downloader_instance * Instance);
    
    typedef boost::function< std::auto_ptr<http_downloader_instance>(http_downloader::DownloadStateNotify) > CreateInstaceFunc;
    typedef boost::function< void () > OnCancelElement;

    Impl( 
        CreateInstaceFunc createInstanceFunc,
        http_downloader::DownloadStateNotify onChangeState);

    virtual ~Impl();

    static void shutdown();
        
private:
    http_downloader_instance * instance_;

    void set_instance(http_downloader_instance * Instance);
    
    void DoDownloadStateNotify(http_downloader::download_state_t State); 
    http_downloader::DownloadStateNotify downloadStateNotify_;
    OnCancelElement onCancelElement_;

};

} } // namespace openmedia::downloader
 
#endif


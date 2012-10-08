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



// precompiled header begin
#include "DTHeadersDownload.h"
// precompiled header end

#include <boost/bind.hpp>

#include <openmedia/DTHttpDownloader.h>
#include "DTDownloaderUtils.h"
#include "DTHttpDownloaderImpl.h"
#include "DTHttpDownloaderInstance.h"
#include <openmedia/DTString.h>

#include <string>

namespace openmedia { namespace downloader {

std::auto_ptr<http_downloader_instance> create_http_downloader_impl_simple(const std::string & Url,
        const std::string & Cookies,
        const std::string & Filename,
        http_downloader::DownloadStateNotify StateNotify, 
        http_downloader::DownloadProgressNotify ProgressNotify);

std::auto_ptr<http_downloader_instance> create_http_downloader_impl_multi(const std::string & Url,
        const std::string & Cookies,
        const std::string & Filename,
        http_downloader::DownloadStateNotify StateNotify, 
        http_downloader::DownloadProgressNotify ProgressNotify, size_t PartSizeBytes);

std::auto_ptr<http_downloader_instance> create_http_downloader_impl_2_simple(const std::string & Url,
        const std::string & Cookies,
        std::ostream & OutputStream,
        http_downloader::DownloadStateNotify StateNotify, 
        http_downloader::DownloadProgressNotify ProgressNotify);

///////////////////////////////////////////////////////////////////////////////////////

void http_downloader::shutdown()
{
    http_downloader::Impl::shutdown();
}

http_downloader::http_downloader(
        const std::string & Url,
        const std::string & Cookies,
        const std::string & Filename,
        http_downloader::DownloadStateNotify StateNotify, 
        http_downloader::DownloadProgressNotify ProgressNotify)
{
    std::string fileName;

#ifdef _WIN32
    std::string ansiName = Filename;
    fileName = ansi_to_utf8(ansiName);
#else
    fileName = Filename;
#endif

    impl_ = new Impl( 
        boost::bind(&create_http_downloader_impl_simple, Url, Cookies, fileName, _1, ProgressNotify), StateNotify
        //create_http_downloader_impl_simple(Url, Cookies, fileName, StateNotify, ProgressNotify).release() 
        );
}

http_downloader::http_downloader(const std::string & Url,
                const std::string & Cookies,
                const std::wstring & Filename,
                DownloadStateNotify StateNotify,
                DownloadProgressNotify ProgressNotify)
{
    std::string fileName = utf16_to_utf8(Filename);

    impl_ = new Impl( 
        boost::bind( &create_http_downloader_impl_simple, Url, Cookies, fileName, _1, ProgressNotify ),  StateNotify
        //create_http_downloader_impl_simple(Url, Cookies, fileName, StateNotify, ProgressNotify).release() 
        );
}

http_downloader::http_downloader(
                                 const std::string & Url,
                                 const std::string & Cookies,
                                 const std::string & Filename,
                                 http_downloader::DownloadStateNotify StateNotify, 
                                 http_downloader::DownloadProgressNotify ProgressNotify,
                                 size_t PartSizeBytes)
{
    std::string fileName;

#ifdef _WIN32
    std::string ansiName = Filename;
    fileName = ansi_to_utf8(ansiName);
#else
    fileName = Filename;
#endif

    impl_ = new Impl( 
        boost::bind(&create_http_downloader_impl_multi, Url, Cookies, fileName, _1, ProgressNotify, PartSizeBytes), StateNotify
        //create_http_downloader_impl_multi(Url, Cookies, fileName, StateNotify, ProgressNotify, PartSizeBytes).release() 
        );
}

http_downloader::http_downloader(const std::string & Url,
                                 const std::string & Cookies,
                                 const std::wstring & Filename,
                                 DownloadStateNotify StateNotify,
                                 DownloadProgressNotify ProgressNotify,
                                 size_t PartSizeBytes)
{
    std::string fileName = utf16_to_utf8(Filename);

    impl_ = new Impl( 
        boost::bind(&create_http_downloader_impl_multi, Url, Cookies, fileName, _1, ProgressNotify, PartSizeBytes), StateNotify
        //create_http_downloader_impl_multi(Url, Cookies, fileName, StateNotify, ProgressNotify, PartSizeBytes).release() 
        );
}

http_downloader::http_downloader(const std::string & Url,
                const std::string & Cookies,
                std::ostream & OutputStream,
                DownloadStateNotify StateNotify,
                DownloadProgressNotify ProgressNotify)
{
    impl_ = new Impl( create_http_downloader_impl_2_simple(Url, Cookies, OutputStream, StateNotify, ProgressNotify).release() );    
}

http_downloader::~http_downloader()
{
    delete impl_;
}

http_downloader::command_result_t http_downloader::pause()
{
    return impl_->pause();
}

http_downloader::command_result_t http_downloader::resume()
{
    return impl_->resume();
}

http_downloader::command_result_t http_downloader::cancel()
{
    return impl_->cancel();
}
    
} }

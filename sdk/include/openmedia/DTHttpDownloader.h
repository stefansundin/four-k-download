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


#ifndef _DT_HTTPDOWNLOADER_H_
#define _DT_HTTPDOWNLOADER_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <iosfwd>

namespace openmedia { namespace downloader {

class http_downloader : boost::noncopyable
{
public:
    enum command_result_t
    {
        resultFail = 0,
        resultOk = 1,
        resultNotFound = 2
    };
    
    enum download_state_t 
    {
        stateNull = 0,
        statePending,
        stateStart,
        stateCancel,
        stateFinish,
        stateDownload,
        statePause,
        stateResume,
        stateError,
        stateCount
    };
    
    typedef boost::function< void (boost::uint64_t) > DownloadProgressNotify;
    typedef boost::function< void (download_state_t) > DownloadStateNotify;

public:
    http_downloader(const std::string & Url,
        const std::string & Cookies,
        std::ostream & OutputStream,
        DownloadStateNotify StateNotify,
        DownloadProgressNotify ProgressNotify);

    http_downloader(const std::string & Url,
        const std::string & Cookies,
        const std::string & Filename,
        DownloadStateNotify StateNotify,
        DownloadProgressNotify ProgressNotify);

    http_downloader(const std::string & Url,
        const std::string & Cookies,
        const std::wstring & Filename,
        DownloadStateNotify StateNotify,
        DownloadProgressNotify ProgressNotify);

    http_downloader(const std::string & Url,
        const std::string & Cookies,
        const std::string & Filename,
        DownloadStateNotify StateNotify,
        DownloadProgressNotify ProgressNotify, 
        size_t PartSizeBytes
        );

    http_downloader(const std::string & Url,
        const std::string & Cookies,
        const std::wstring & Filename,
        DownloadStateNotify StateNotify,
        DownloadProgressNotify ProgressNotify,
        size_t PartSizeBytes
        );

    ~http_downloader();

    command_result_t pause();
    command_result_t resume();
    command_result_t cancel();

    static void shutdown();

public:
    class Impl;

private:
    Impl * impl_;    
};

} }

#endif

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


#ifndef _DT_HTTPDOWNLOADER2_H_
#define _DT_HTTPDOWNLOADER2_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <iosfwd>

#include <openmedia/DTController.h>

namespace openmedia { 
namespace http {

class Writer
{
public:
    virtual bool valid() const = 0;
    virtual void write(const char * buf, std::size_t bufSize) = 0;
    virtual ~Writer() = 0;
};

inline Writer::~Writer() {}

enum DownloadState
{
    downloadStatePending,
    downloadStateCancel,
    downloadStateFinish,
    downloadStateDownload,
    downloadStatePause,
    downloadStateError
};
    
typedef boost::function< void (boost::uint64_t) > OnDownloadProgress;
typedef boost::function< void (DownloadState) > OnDownloadState;

struct DownloadCallback
{
    OnDownloadState onDownloadState;
    OnDownloadProgress onDownloadProgress;
};

class DownloadService;
typedef boost::shared_ptr<DownloadService> DownloadServicePtr;

DownloadServicePtr defaultDownloadService();

ControllerPtr download(DownloadServicePtr downloadService, 
                       const std::string & url,
                       Writer & outputStream,
                       DownloadCallback downloadCallback);

ControllerPtr download(DownloadServicePtr downloadService, 
                       const std::string & url,
                       const std::string & fileName,
                       DownloadCallback downloadCallback);

ControllerPtr download(DownloadServicePtr downloadService, 
                       const std::string & url,
                       const std::wstring & fileName,
                       DownloadCallback downloadCallback);

}
}

#endif

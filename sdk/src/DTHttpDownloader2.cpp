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


#include "DTHeadersDownload.h"
#include <openmedia/DTHttpDownloader2.h>

#include <boost/asio/io_service.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/make_shared.hpp>
#include <boost/bind.hpp>

#include <urdl/read_stream.hpp>
#include <urdl/istream.hpp>
#include <urdl/http.hpp>

#include <openmedia/DTController.h>
#include <openmedia/DTString.h>

#include "DTControllerImpl.h"
#include "DTIOService.h"

#include <fstream>

namespace openmedia { 
namespace http {

class DownloadService
{
public:
    virtual boost::shared_ptr<boost::asio::io_service> getIoService() = 0;
};

class DownloadServiceDefault : public DownloadService
{
public:
    virtual boost::shared_ptr<boost::asio::io_service> getIoService()
    {
        return openmedia::downloader::io_service_provider::get()->get_io_service();
    }
};

DownloadServicePtr defaultDownloadService()
{
    return DownloadServicePtr( new DownloadServiceDefault() );
}

class DownloaderInstance
    : public boost::enable_shared_from_this<DownloaderInstance>
{
public:
    DownloaderInstance(boost::asio::io_service& io_service,
        const urdl::url& url,
        Writer& outputStream, 
        DownloadCallback downloadCallback,
        boost::shared_ptr<Controller::Impl> controller)
        : read_stream_(io_service),
        url_(url),
        ostream_(outputStream),
        downloadCallback_(downloadCallback),
        readed_(0),
        controller_(controller)
    {
    }

    void start()
    {
        read_stream_.async_open(url_,
            boost::bind(&DownloaderInstance::handle_open,
            shared_from_this(), _1));
    }

private:
    void handle_open(const boost::system::error_code& ec)
    {
        if (!ec)
        {
            notifyState(downloadStateDownload);

            read_stream_.async_read_some(
                boost::asio::buffer(buffer_),
                boost::bind(&DownloaderInstance::handle_read,
                shared_from_this(), _1, _2));
        }
        else
        {
            notifyState(downloadStateError);                    
        }
    }

    void handle_read(const boost::system::error_code& ec, std::size_t length)
    {
        if (checkCancel())
        {
            notifyState(downloadStateCancel);
            return;
        }

        if (!ec)
        {
            notifyProgress(readed_ += length);
            ostream_.write(buffer_, length);
            read_stream_.async_read_some(
                boost::asio::buffer(buffer_),
                boost::bind(&DownloaderInstance::handle_read,
                shared_from_this(), _1, _2));
        }
        else
        {
            if ( boost::asio::error::misc_category == ec.category() &&
                boost::asio::error::eof == ec.value()
                )
            {
                notifyState(downloadStateFinish);
            }
            else
            {
                notifyState(downloadStateError);
            }
        }
    }

    bool checkCancel() const
    {
        if (controller_)
            return controller_->isCancel();
        else
            return false;
    }

    void notifyState(DownloadState state) const 
    {
        if (downloadCallback_.onDownloadState)
            downloadCallback_.onDownloadState(state);        
    }

    void notifyProgress(boost::uint64_t progress) const 
    {
        if (downloadCallback_.onDownloadProgress)
            downloadCallback_.onDownloadProgress(progress);
    }

    urdl::read_stream read_stream_;
    urdl::url url_;
    Writer & ostream_;
    DownloadCallback downloadCallback_;
    boost::uint64_t readed_;
    boost::shared_ptr<Controller::Impl> controller_;
    char buffer_[65535];
};

typedef boost::shared_ptr<DownloaderInstance> DownloaderInstancePtr;

ControllerPtr download(DownloadServicePtr downloadService, 
                       const std::string & url,
                       Writer & outputStream,
                       DownloadCallback downloadCallback)
{
    boost::shared_ptr<Controller::Impl> cntr( new Controller::Impl() );

    DownloaderInstancePtr d = DownloaderInstancePtr(
        new DownloaderInstance(
        *(downloadService->getIoService()),
        url,
        outputStream,
        downloadCallback,
        cntr
        ) );
    d->start();
    return ControllerPtr( new Controller(cntr) );
}

class WriterFile : public Writer
{
public:
    WriterFile(const std::string & fileName) : file_(NULL)
    {
        file_ = fopen(fileName.c_str(), "wb");
    }

    WriterFile(const std::wstring & fileName) : file_(NULL)
    {
#ifndef _WIN32
        file_ = fopen(utf16_to_utf8(fileName).c_str(), "wb");
#else
        file_ = _wfopen(fileName.c_str(), L"wb");
#endif
    }

    virtual bool valid() const
    {
        return file_ != NULL; 
    }

    virtual void write(const char * buf, std::size_t bufSize)
    {
        if (file_)
            fwrite((const void*)buf, bufSize, 1, file_);
    }

    void close()
    {
        if (file_)
        {
            fclose(file_);
            file_ = NULL;
        }
    }

    virtual ~WriterFile()
    {
        close();
    }

private:
    FILE * file_;

};


class DownloadFile : public boost::enable_shared_from_this<DownloadFile>
{
public:
    template <class Str>
    DownloadFile(const Str & fileName) : file_( fileName )
    {           
    }

    ~DownloadFile()
    {
        file_.close();
#if defined(_DEBUG) && 0
        std::cerr << "destroy DownloadFile\n";
#endif
    }

    ControllerPtr download(DownloadServicePtr downloadService,
        const std::string & url,
        DownloadCallback downloadCallback)
    {
        downloadCallback_ = downloadCallback;
        DownloadCallback callback = 
        {
            boost::bind(&DownloadFile::onDownloadState, shared_from_this(), _1),
            boost::bind(&DownloadFile::onDownloadProgress, shared_from_this(), _1)
        };

        return ::openmedia::http::download(downloadService, url, file_, callback); 
    }

private:
    void onDownloadState(DownloadState state)
    {
        if (downloadCallback_.onDownloadState)
            downloadCallback_.onDownloadState(state);
    }

    void onDownloadProgress(boost::uint64_t progress)
    {
        if (downloadCallback_.onDownloadProgress)
            downloadCallback_.onDownloadProgress(progress);
    }

    WriterFile file_;
    DownloadCallback downloadCallback_;
};

ControllerPtr download(DownloadServicePtr downloadService, 
                       const std::string & url,
                       const std::string & fileName,
                       DownloadCallback downloadCallback)
{
    boost::shared_ptr<DownloadFile> d( new DownloadFile(fileName) );
    return d->download(downloadService, url, downloadCallback);
}

ControllerPtr download(DownloadServicePtr downloadService, 
                       const std::string & url,
                       const std::wstring & fileName,
                       DownloadCallback downloadCallback)
{
    boost::shared_ptr<DownloadFile> d( new DownloadFile(fileName) );
    return d->download(downloadService, url, downloadCallback);
}

}
}

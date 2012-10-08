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

#include <openmedia/DTCommon.h>
#include <openmedia/DTHttpDownloader.h>
#include "DTDownloaderUtils.h"
#include "DTHttpDownloaderInstance.h"
#include <openmedia/DTString.h>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/filesystem/fstream.hpp>
#include <urdl/read_stream.hpp>
#include <urdl/istream.hpp>
#include <urdl/http.hpp>

#include "DTLog.h"

namespace openmedia { namespace downloader {

class http_downloader_simple;

class downloader_instance_simple
    : public boost::enable_shared_from_this<downloader_instance_simple>, boost::noncopyable
{
public:
    downloader_instance_simple(boost::asio::io_service& io_service, http_downloader_simple * Owner)
        : read_stream_(io_service),
        owner_(Owner),
        readed_bytes_(0)
    {
    }

    void start(const urdl::url& url, const std::string & Cookies, const std::wstring& file)
    {
        file_ = file;
        read_stream_.set_option(urdl::http::user_agent(userAgent));

        urdl::http::additional_headers additionalHeaders;

        if (Cookies.length())
        {
            additionalHeaders.get().push_back(
                urdl::http::additional_headers::header("Cookie", Cookies)
                );

            read_stream_.set_option(additionalHeaders);
        }

        read_stream_.async_open(url,
            boost::bind(&downloader_instance_simple::handle_open,
            shared_from_this(), _1));
    }

private:

    http_downloader_simple * owner()
    {
        return owner_;
    }

private:
    void handle_open(const boost::system::error_code& ec);
    void handle_read(const boost::system::error_code& ec, std::size_t length);

    urdl::read_stream read_stream_;
    std::wstring file_;
    boost::filesystem::ofstream ofstream_;
    //char buffer_[1024];
    char buffer_[65536];
    http_downloader_simple * owner_;
    boost::uint64_t readed_bytes_;
};

class http_downloader_simple : public http_downloader_instance
{
    friend class downloader_instance_simple;
public:

    http_downloader_simple(const std::string & Url,
        const std::string & Cookies,
        const std::string & Filename,
        http_downloader::DownloadStateNotify StateNotify,
        http_downloader::DownloadProgressNotify ProgressNotify);

    virtual ~http_downloader_simple();

    virtual http_downloader::command_result_t pause();
    virtual http_downloader::command_result_t resume();
    virtual http_downloader::command_result_t cancel();

private:
    boost::asio::io_service io_service_; 

    http_downloader::DownloadStateNotify state_notify_;
    http_downloader::DownloadProgressNotify progress_notify_;

    std::string url_;
    std::string cookies_;
    std::wstring filename_;

    
    boost::shared_ptr<boost::thread> thread_;

    enum State
    {
        stateNull = 0,
        stateDownload = 1,
        statePaused = 2
    };

    State state_;

    enum Command
    {
        commandNull = 0,
        commandPause,
        commandResume,
        commandCancel                    
    };

    Command command_;

    boost::mutex stateGuard_;

};


void downloader_instance_simple::handle_open(const boost::system::error_code& ec)
{
    if (!ec)
    {
        ofstream_.open(file_, (std::ios_base::out | std::ios_base::binary | std::ios_base::trunc) );

		if (!ofstream_)
		{
	        if (owner()->state_notify_)
	            owner()->state_notify_(http_downloader::stateError);
			return;
		}
		
        read_stream_.async_read_some(
            boost::asio::buffer(buffer_),
            boost::bind(&downloader_instance_simple::handle_read,
            shared_from_this(), _1, _2));

        owner()->state_ = http_downloader_simple::stateDownload;

        if (owner()->state_notify_)
            owner()->state_notify_(http_downloader::stateStart);
    }
    else
    {
        if (owner()->state_notify_)
            owner()->state_notify_(http_downloader::stateError);
    }
}

void downloader_instance_simple::handle_read(const boost::system::error_code& ec, std::size_t length)
{
    if (!ec)
    {
        ofstream_.write(buffer_, length);
        readed_bytes_ += length;
        
        if (owner()->progress_notify_)
            owner()->progress_notify_(readed_bytes_);


        {
            boost::mutex::scoped_lock lock(owner()->stateGuard_);

            while (owner()->command_ != http_downloader_simple::commandNull)
            {
                switch (owner()->command_)
                {
                case http_downloader_simple::commandPause:
                    {
                        owner()->state_ = http_downloader_simple::statePaused;
                        owner()->command_ = http_downloader_simple::commandNull;

                        if (owner()->state_notify_)
                            owner()->state_notify_(http_downloader::statePause);

                        DT_LOG(trace) << "state paused" << "\n";
                        while (
                            owner()->command_ != http_downloader_simple::commandCancel &&
                            owner()->command_ != http_downloader_simple::commandResume
                            )   
                        {
                            lock.unlock();
                            boost::this_thread::sleep( boost::posix_time::milliseconds(100) ); 
                            lock.lock();                                                                                                
                        }
                        DT_LOG(trace) << "leave paused" << "\n";

                        if (owner()->command_ == http_downloader_simple::commandResume)
                        {
                            owner()->state_ = http_downloader_simple::stateDownload;
                            owner()->command_ = http_downloader_simple::commandNull;
                            if (owner()->state_notify_)
                                owner()->state_notify_(http_downloader::stateResume);
                        }

                    }
                    break;

                case http_downloader_simple::commandCancel:
                    {
                        DT_LOG(trace) << "cancel" << "\n";
                        owner()->command_ = http_downloader_simple::commandNull;
                        ofstream_.close();
                        if (owner()->state_notify_)
                            owner()->state_notify_(http_downloader::stateCancel);
                        return;
                    }
                    break;

                case http_downloader_simple::commandResume:
                    break;

                }
            }
            
        }

        read_stream_.async_read_some(
            boost::asio::buffer(buffer_),
            boost::bind(&downloader_instance_simple::handle_read,
            shared_from_this(), _1, _2));
    } 
    else
    {
        ofstream_.close();
        if ( boost::asio::error::misc_category == ec.category() )
        {
            if (boost::asio::error::eof == ec.value())
            {
                if (owner()->state_notify_)
                    owner()->state_notify_(http_downloader::stateFinish);    
            }
        }
        else
        {
                if (owner()->state_notify_)
                    owner()->state_notify_(http_downloader::stateError);    
        }
    }
}


http_downloader_simple::http_downloader_simple(
        const std::string & Url,
        const std::string & Cookies,
        const std::string & Filename,
        http_downloader::DownloadStateNotify StateNotify, 
        http_downloader::DownloadProgressNotify ProgressNotify)
        : 

io_service_(), 
url_(Url),
cookies_(Cookies),
filename_( utf8_to_utf16(Filename) ),
state_notify_(StateNotify),
progress_notify_(ProgressNotify),
command_(http_downloader_simple::commandNull),
state_(stateNull)
{
    { 
        boost::shared_ptr<downloader_instance_simple> downloader = 
            boost::shared_ptr<downloader_instance_simple>( new downloader_instance_simple(io_service_, this) );
        downloader->start(Url, Cookies, filename_);
    }
    thread_ = boost::shared_ptr< boost::thread >( new boost::thread(boost::bind(&boost::asio::io_service::run, boost::ref(io_service_)) ));    
    
}

http_downloader_simple::~http_downloader_simple()
{
    DT_LOG(trace) << "destroy downloader" << "\n";
    this->cancel();
    thread_->join();   
}


http_downloader::command_result_t http_downloader_simple::pause()
{
    boost::mutex::scoped_lock lock(stateGuard_);
    DT_LOG(trace) << "try to put paused" << "\n";
    if (state_ != statePaused)
    {
        DT_LOG(trace) << "put commandPause" << "\n";
        command_ = commandPause;
    }
    return http_downloader::resultOk;
}

http_downloader::command_result_t http_downloader_simple::resume()
{
    boost::mutex::scoped_lock lock(stateGuard_);
    DT_LOG(trace) << "try to put resume" << "\n";
    if (state_ != stateDownload)
    {
        DT_LOG(trace) << "put commandResume" << "\n";
        command_ = commandResume;
    }
    return http_downloader::resultOk;

}

http_downloader::command_result_t http_downloader_simple::cancel()
{
    boost::mutex::scoped_lock lock(stateGuard_);
    DT_LOG(trace) << "put commandCancel" << "\n";
    command_ = commandCancel;
//  io_service_.stop();
//  thread_->join();
    return http_downloader::resultOk;

}

std::auto_ptr<http_downloader_instance> create_http_downloader_impl_simple(const std::string & Url,
        const std::string & Cookies,
        const std::string & Filename,
        http_downloader::DownloadStateNotify StateNotify, 
        http_downloader::DownloadProgressNotify ProgressNotify)
{
    return std::auto_ptr<http_downloader_instance>( new http_downloader_simple(Url, Cookies, Filename, StateNotify, ProgressNotify) );
}

} }

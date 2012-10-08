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

#include <iostream>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared.hpp>
#define BOOST_THREAD_USE_LIB
#include <boost/thread.hpp>
#include <boost/timer.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/lexical_cast.hpp>

#include <urdl/read_stream.hpp>
#include <urdl/istream.hpp>
#include <urdl/http.hpp>

#include <openmedia/DTCommon.h>
#include <openmedia/DTHttpDownloader.h>
#include <openmedia/DTString.h>

#include "DTDownloaderUtils.h"
#include "DTHttpDownloaderInstance.h"
#include "DTIOService.h"
#include "DTLog.h"

namespace openmedia { namespace downloader {

class http_downloader_multi;

class downloader_instance_multi
    : public boost::enable_shared_from_this<downloader_instance_multi>, boost::noncopyable
{
public:
    downloader_instance_multi( boost::shared_ptr< boost::asio::io_service > io_service, http_downloader_multi * Owner)
        : 
        io_service_(io_service),
        read_stream_(*io_service),
        owner_(Owner),
        readed_bytes_(0),
        firstOpen_(true),
        lastRead_(false)
    {
        DT_LOG(trace) << "[downloader_instance_multi::downloader_instance_multi] construct\n";
    }

    void start(const urdl::url& url, const std::string & Cookies, boost::int64_t startByte, boost::int64_t endByte, const std::wstring& file);
    virtual ~downloader_instance_multi()
    {
        DT_LOG(trace) << "[downloader_instance_multi::~downloader_instance_multi] destruct\n";
    }

private:

    http_downloader_multi * owner()
    {
        return owner_;
    }

private:
    void handle_open(const boost::system::error_code& ec);
    void handle_read2(bool Paused, const boost::system::error_code& ec, std::size_t length);

    boost::shared_ptr< boost::asio::io_service > io_service_;
    urdl::read_stream read_stream_;
    
    http_downloader_multi * owner_;
    boost::uint64_t readed_bytes_;
    bool firstOpen_;

    std::wstring file_;
    
    boost::shared_ptr<FILE> outputFile_;

    char buffer_[65535];
    urdl::url url_;
    std::string cookies_;

    bool lastRead_;

};

class http_downloader_multi : public http_downloader_instance
{
    friend class downloader_instance_multi;
public:

    /// \param  Filename[in]    UTF-8 string for ALL paltforms
    http_downloader_multi(const std::string & Url,
        const std::string & Cookies,
        const std::string & Filename,
        http_downloader::DownloadStateNotify StateNotify,
        http_downloader::DownloadProgressNotify ProgressNotify, size_t PartSizeBytes);

    virtual ~http_downloader_multi();

    virtual http_downloader::command_result_t pause();
    virtual http_downloader::command_result_t resume();
    virtual http_downloader::command_result_t cancel();

private:
    enum State
    {
        stateNull = 0,
        stateDownload = 1,
        statePaused = 2
    };

    enum Command
    {
        commandNull = 0,
        commandPause,
        commandResume,
        commandCancel                    
    };
 
    std::string url_;
    std::string cookies_;
    std::wstring filename_; 

    http_downloader::DownloadStateNotify state_notify_;
    http_downloader::DownloadProgressNotify progress_notify_;

    Command command_;
    State state_;
    boost::mutex stateGuard_;
    size_t partSizeBytes_;
    boost::weak_ptr<downloader_instance_multi> instance_weak_;

public:
    void state(State _State); ///< set State
    State state() const; ///< get State
    Command command() const; ///< get Command
    void command(Command _Command); // set Command
    void resetCommand();

private:
    void stateNotify(http_downloader::download_state_t State);
    void progressNotify(boost::uint64_t Value);
    size_t partSizeBytes() const;
};

void http_downloader_multi::state(http_downloader_multi::State _State)
{
    state_ = _State;
}

http_downloader_multi::State http_downloader_multi::state() const
{
    return state_;
}

http_downloader_multi::Command http_downloader_multi::command() const
{
    return command_;
}

void http_downloader_multi::resetCommand()
{
    command_ = commandNull;
}

void http_downloader_multi::command(http_downloader_multi::Command _Command)
{
    command_ = _Command;
}

size_t http_downloader_multi::partSizeBytes() const 
{
    return partSizeBytes_;
}


void downloader_instance_multi::start(const urdl::url& url, const std::string & Cookies, boost::int64_t startByte, boost::int64_t endByte, const std::wstring& file)
{
    url_ = url;
    cookies_ = Cookies;
    file_ = file;
    read_stream_.set_option(urdl::http::user_agent( ::openmedia::downloader::userAgent ));

    urdl::http::additional_headers additionalHeaders;

    if (!Cookies.empty())
        additionalHeaders.get().push_back( urdl::http::additional_headers::header("Cookie", Cookies) );

    const std::string bytesRequest = std::string("bytes=") + boost::lexical_cast<std::string>(startByte) 
        + "-" + (endByte > 0 ? boost::lexical_cast<std::string>(endByte) : "");
        
    additionalHeaders.get().push_back( urdl::http::additional_headers::header("Range", bytesRequest) );

    DT_LOG(trace) << "range " << bytesRequest << "\n";
    
    read_stream_.set_option(additionalHeaders);
    
    read_stream_.async_open(url,
        boost::bind(&downloader_instance_multi::handle_open, shared_from_this(), _1));
    }

void downloader_instance_multi::handle_open(const boost::system::error_code& ec)
{
    DT_LOG(trace) << "handle_open" << ec.value() << "\n";

    if (lastRead_)
    {
        outputFile_.reset();
        owner()->stateNotify(http_downloader::stateFinish);
        return;
    }

    const std::string headers = read_stream_.headers();
    std::vector<HttpHeader> parsedHeaders;
    parse_http_headers(headers, parsedHeaders); 
    
    HttpContentRange range;
    bool haveRange = get_content_range(parsedHeaders, range);

    if (haveRange && (range.Last == range.Size - 1) )
        lastRead_ = true;
    
    if ( urdl::http::errc::partial_content == ec.value() || !ec )
    {
        if (firstOpen_)
        {
#if defined(DT_CONFIG_HAVE_UTF16_OPEN) && (1 == DT_CONFIG_HAVE_UTF16_OPEN)
            outputFile_ = boost::shared_ptr<FILE>( _wfopen(file_.c_str(), L"wb"), &fclose );
#else
            ::std::string fileUtf8 = utf16_to_utf8(file_);
            outputFile_ = boost::shared_ptr<FILE>( fopen(fileUtf8.c_str(), "wb"), &fclose );
#endif
            if (!outputFile_.get())
            {
                owner()->stateNotify(http_downloader::stateError);
			    return;
            }

            firstOpen_ = false;
        }

        read_stream_.async_read_some(
            boost::asio::buffer(buffer_),
            boost::bind(&downloader_instance_multi::handle_read2, shared_from_this(), false, _1, _2));

        owner()->state(http_downloader_multi::stateDownload);
        owner()->stateNotify(http_downloader::stateStart);
    }
    else
    {
        outputFile_.reset();
        
        if (urdl::http::errc::requested_range_not_satisfiable == ec.value() || 
            urdl::http::errc::not_found == ec.value() // dailymotion !
            )
        {
            owner()->stateNotify(http_downloader::stateFinish);
        }
        else
        {
            DT_LOG(error) << "downloader_instance_multi::handle_open error: " << ec.value() << "\n";
            owner()->stateNotify(http_downloader::stateError);
        }
    }
}

void downloader_instance_multi::handle_read2(bool Paused, const boost::system::error_code& ec, std::size_t length)
{
    DT_LOG(trace) << "handle_read: " << ec.value() << "\n";

    if (Paused)
    {
        boost::mutex::scoped_lock lock(owner()->stateGuard_);
        if ( owner()->command() == http_downloader_multi::commandCancel)
        {
            DT_LOG(trace) << "cancel" << "\n";
            owner()->resetCommand();
            outputFile_.reset();
            owner()->stateNotify(http_downloader::stateCancel);
            return;
        } 
        else if ( owner()->command() == http_downloader_multi::commandResume )
        {
            DT_LOG(trace) << "leave paused" << "\n";
            owner()->state( http_downloader_multi::stateDownload );
            owner()->resetCommand();
            owner()->stateNotify(http_downloader::stateResume);

            read_stream_.async_read_some(
                boost::asio::buffer(buffer_),
                boost::bind(&downloader_instance_multi::handle_read2, shared_from_this(), false, _1, _2));
            return;
        }
        else // still paused
        {
            boost::this_thread::sleep( boost::posix_time::milliseconds(10) ); 
            read_stream_.get_io_service().post(  
                boost::bind(&downloader_instance_multi::handle_read2, shared_from_this(), true, boost::system::error_code() , 0) );
            return;
        }
    }

    if (!ec)
    {
        DT_LOG(trace) << "NORMAL: " << length << "\n";
        
        if (outputFile_)
            fwrite((const void*)buffer_, length, 1, outputFile_.get());

        readed_bytes_ += length;
        DT_LOG(trace) << "[send progressNotify] ...\n";
        owner()->progressNotify(readed_bytes_);
        DT_LOG(trace) << "[send progressNotify] ok \n";

        {
            boost::mutex::scoped_lock lock(owner()->stateGuard_);
            while (owner()->command() != http_downloader_multi::commandNull)
            {
                switch ( owner()->command() )
                {
                case http_downloader_multi::commandPause:
                    {
                        owner()->state( http_downloader_multi::statePaused );
                        owner()->resetCommand();
                        owner()->stateNotify(http_downloader::statePause);

                        DT_LOG(trace) << "state paused" << "\n";
                        read_stream_.get_io_service().post(  
                            boost::bind(&downloader_instance_multi::handle_read2, shared_from_this(), true, boost::system::error_code() , 0) 
                            );
                        return;
                    }
                    break;

                case http_downloader_multi::commandCancel:
                    {
                        DT_LOG(trace) << "cancel" << "\n";
                        owner()->resetCommand();
                        outputFile_.reset();
                        owner()->stateNotify(http_downloader::stateCancel);
                        return;
                    }
                    break;

                case http_downloader_multi::commandResume:
                    break;

                }
            }
            
        }
        
        read_stream_.async_read_some(
            boost::asio::buffer(buffer_),
            boost::bind(&downloader_instance_multi::handle_read2, shared_from_this(), false, _1, _2));
    } 
    else
    {
        if ( boost::asio::error::misc_category == ec.category() && boost::asio::error::eof == ec.value())
        {
            // read next part
            read_stream_.close();

            const boost::int64_t startPos = readed_bytes_;
            const size_t partSizeBytes = owner()->partSizeBytes();
            const boost::int64_t endPos = readed_bytes_ + partSizeBytes;

            DT_LOG(trace) << "PART: s" << startPos << " e: " << endPos << " sz: " << partSizeBytes << "\n";
            this->start(url_, cookies_, startPos, endPos, L"");
        }
        else
        {
            DT_LOG(trace) << "ERROR: " << "\n";
            DT_LOG(error) << "downloader_instance_multi::handle_read error: " << ec.value() << "\n";
            owner()->stateNotify(http_downloader::stateError);
        }
    }
}


void http_downloader_multi::stateNotify(http_downloader::download_state_t State)
{
    if (state_notify_)
        state_notify_(State);
}

void http_downloader_multi::progressNotify(boost::uint64_t Value)
{
    if (progress_notify_)
        progress_notify_(Value);
}

http_downloader_multi::http_downloader_multi(
        const std::string & Url,
        const std::string & Cookies,
        const std::string & Filename,
        http_downloader::DownloadStateNotify StateNotify, 
        http_downloader::DownloadProgressNotify ProgressNotify, size_t PartSizeBytes)
        : 
url_(Url),
cookies_(Cookies),
filename_( utf8_to_utf16(Filename) ),
state_notify_(StateNotify),
progress_notify_(ProgressNotify),
command_(http_downloader_multi::commandNull),
state_(stateNull),
partSizeBytes_(PartSizeBytes)
{
    {
        boost::shared_ptr<downloader_instance_multi> downloader = 
            boost::make_shared<downloader_instance_multi>(io_service_provider::get()->get_io_service() , this);

        instance_weak_ = downloader;
        downloader->start(Url, Cookies, 0, PartSizeBytes, filename_);
    }
}


http_downloader_multi::~http_downloader_multi()
{
    DT_LOG(trace) << "[http_downloader_multi::~http_downloader_multi] enter" << "\n";
    this->cancel();
    //thread_.join();   
    DT_LOG(trace) << "[http_downloader_multi::~http_downloader_multi] leave" << "\n";
}

http_downloader::command_result_t http_downloader_multi::pause()
{
    boost::mutex::scoped_lock lock(stateGuard_);
    DT_LOG(trace) << "[http_downloader_multi::pause]: try to put paused..." << "\n";
    if (state() != statePaused)
    {
        DT_LOG(trace) << "[http_downloader_multi::pause]: put commandPause" << "\n";
        command(commandPause);
    }
    return http_downloader::resultOk;
}

http_downloader::command_result_t http_downloader_multi::resume()
{
    boost::mutex::scoped_lock lock(stateGuard_);
    DT_LOG(trace) << "try to put resume" << "\n";
    if (state() != stateDownload)
    {
        DT_LOG(trace) << "put commandResume" << "\n";
        command(commandResume);
    }
    return http_downloader::resultOk;

}

http_downloader::command_result_t http_downloader_multi::cancel()
{
    {
        boost::mutex::scoped_lock lock(stateGuard_);
        DT_LOG(trace) << "put commandCancel" << "\n";
        command(commandCancel);
    }

    while (!instance_weak_.expired())
    {
        boost::this_thread::sleep( boost::posix_time::milliseconds(100) );
    }
//  io_service_.stop();
//  thread_->join();
    return http_downloader::resultOk;

}

std::auto_ptr<http_downloader_instance> create_http_downloader_impl_multi(const std::string & Url,
        const std::string & Cookies,
        const std::string & Filename,
        http_downloader::DownloadStateNotify StateNotify, 
        http_downloader::DownloadProgressNotify ProgressNotify, size_t PartSizeBytes)
{
    return std::auto_ptr<http_downloader_instance>( new http_downloader_multi(Url, Cookies, Filename, StateNotify, ProgressNotify, PartSizeBytes) );
}

} }

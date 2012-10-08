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

#define BOOST_THREAD_USE_LIB

#include "DTIOService.h"

#include <boost/thread/mutex.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/thread.hpp>
#include <boost/smart_ptr/make_shared.hpp>

namespace openmedia { namespace downloader {

namespace {
    static boost::mutex create_lock_;
}

io_service_provider * io_service_provider::instance_ = NULL;

class io_service_provider::Impl
{
public:
    Impl()
    {
        io_service_ = boost::make_shared<boost::asio::io_service>();
        thread_ = new boost::thread(boost::bind(&io_service_provider::Impl::do_run, this));
    }
    
    ~Impl()
    {
        io_service_->stop();
        thread_->join();
        delete thread_;    
    }

    boost::shared_ptr<boost::asio::io_service> get_io_service()
    {
        return io_service_;
    }

private:
    void do_run()
    {
        boost::asio::io_service::work work(*io_service_);
        io_service_->run();        
    }

private:
    boost::shared_ptr< boost::asio::io_service > io_service_; 
    boost::thread * thread_;

};

io_service_provider * io_service_provider::get()
{

    if (!instance_)
    {
        boost::mutex::scoped_lock lock(create_lock_);
        if (!instance_)
        {
            io_service_provider * temp = new io_service_provider();
            instance_ = temp;                
        }
    }

    return instance_;
}

io_service_provider::io_service_provider() : impl_( new io_service_provider::Impl() )
{}

io_service_provider::~io_service_provider()
{
    delete impl_;
}

boost::shared_ptr<boost::asio::io_service> io_service_provider::get_io_service()
{
    return impl_->get_io_service();
}

}
}

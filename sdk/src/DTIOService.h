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


#ifndef _DT_IOSERVICE_H_
#define _DT_IOSERVICE_H_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio/io_service.hpp>

namespace openmedia { namespace downloader {

class io_service_provider : boost::noncopyable
{
public:
    static io_service_provider * get();
    boost::shared_ptr<boost::asio::io_service> get_io_service();
    ~io_service_provider();

private:
    io_service_provider();
    static io_service_provider * instance_;

    class Impl;
    Impl * impl_;
};

}
}

#endif

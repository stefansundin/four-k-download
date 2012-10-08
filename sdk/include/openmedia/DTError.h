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


#ifndef _DTERROR_H_INCLUDED_
#define _DTERROR_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <exception>
#include <boost/exception/all.hpp>
#include <boost/throw_exception.hpp>

#if defined(_WINDOWS) 
#include <excpt.h> 
#endif 

namespace openmedia {
namespace errors {

class dt_error :
	public virtual boost::exception,
	public virtual ::std::exception
{
public:
	virtual char const * what() const throw();
    ~dt_error() throw();

protected:
    std::string m_msg;

protected:
    dt_error(const char * msg = "openmedia sdk error");
};

struct error: virtual dt_error {};

struct invalid_argument: virtual dt_error 
{
    invalid_argument(): dt_error("invalid argument")
    {}
};

struct invalid_pointer: virtual dt_error
{
    invalid_pointer(): dt_error("invalid pointer")
    {}
};

struct unexpected: virtual dt_error 
{
    unexpected(): dt_error("unexpected")
    {}
};

struct std_exception: virtual dt_error 
{};

struct bad_alloc: virtual dt_error 
{
    bad_alloc(): dt_error("bad alloc")
    {}
};

struct invalid_operation: virtual dt_error 
{
    invalid_operation(): dt_error("invalid operation")
    {}
};

} // namespace errors
} // namespace openmedia

#endif

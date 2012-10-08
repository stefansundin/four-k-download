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
#include "DTHeadersCommon.h"
// precompiled header end

/// \file   DTError.cpp

#include <openmedia/DTError.h>
#include <string>

namespace openmedia {
namespace errors {

dt_error::dt_error(const char * msg): m_msg(msg)
{}

char const * dt_error::what() const throw()
{
    return m_msg.c_str();
}

dt_error::~dt_error() throw()
{
}

}
}

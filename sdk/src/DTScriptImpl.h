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


/// \file   DTScriptImpl.h

#ifndef _DTSCRIPTIMPL_H_INCLUDED_
#define _DTSCRIPTIMPL_H_INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <openmedia/DTCommon.h>
#include <openmedia/DTScript.h>

namespace openmedia { namespace script { 

/// \class script::Impl
class script::Impl : impl_base
{
public:
    virtual std::string execute(const ::std::string & Name, const params & Params) = 0;
   
};

} } 

#endif

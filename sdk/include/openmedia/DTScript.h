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


/// \file   DTScript.h

#ifndef _DTSCRIPT_H_INCLUDED_
#define _DTSCRIPT_H_INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <openmedia/DTTypes.h>
#include <openmedia/DTDeclareImpl.h>
#include <openmedia/DTCommon.h>
#include <openmedia/DTError.h>
#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

namespace openmedia { namespace script { 

class script;
typedef boost::shared_ptr<script> script_ptr;

typedef ::std::pair< ::std::string, ::std::string > string_string_pair;
typedef ::std::vector<  string_string_pair > params; 

enum errorCode 
{
    errorInknown = 0,
    errorPending = 1
};

enum scriptType
{
    scriptAuto = 0,
    scriptPython = 1,
    scriptQt
};

/// \class script
class _OPENMEDIASDK_API script : interface_base
{
public:
    std::string execute(const ::std::string & Name, const params & Params);
    void shutdown();

public:

    class error : public ::openmedia::errors::dt_error
    {};

public:
    virtual ~script();

public:
    class Impl;

protected:
    script(script::Impl * _Impl);
    void reset_impl(Impl * _Impl);

private:
    Impl * impl_;
};

script_ptr create(scriptType Type, const std::string & Program);

} }

#endif //#ifndef _DTSCRIPT_H_INCLUDED_

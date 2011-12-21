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




#include <openmedia/DTHeaders.h>


#include <openmedia/DTScript.h>
#include "DTScriptImpl.h"

namespace openmedia { namespace script { 

std::string script::execute(const ::std::string & Name, const params & Params)
{
    if (!impl_)
        throw script::error();
    return impl_->execute(Name, Params);
}

void script::shutdown()
{
    delete impl_;
    impl_ = NULL;
}

script::script(script::Impl * _Impl) : impl_(_Impl)
{
    DT_STRONG_ASSERT(NULL != _Impl);
}

script::~script()
{
    delete impl_;
}

void script::reset_impl(script::Impl * _Impl)
{
    delete impl_;
    DT_STRONG_ASSERT(NULL != _Impl);
    impl_ = _Impl;
}

} } // namespace openmedia::script

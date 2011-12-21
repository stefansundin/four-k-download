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


/// \file   DTMetadataTagImpl.h

#ifndef _DTMETADATATAGIMPL_H_INCLUDED_
#define _DTMETADATATAGIMPL_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <string>

#include <openmedia/DTCommon.h>

namespace openmedia {

/// \class  metadatatag_impl
class metadata_tag::Impl: public impl_base
{
public:
    virtual std::string     get_key() = 0;
    virtual std::string     get_value() = 0;
};

} // namespace openmedia 

#endif

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


/// \file   DTMetadataImpl.h

#ifndef _DTMETADATAIMPL_H_INCLUDED_
#define _DTMETADATAIMPL_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <openmedia/DTCommon.h>

namespace openmedia {

/// \class  metadata::Impl
class metadata::Impl: public impl_base
{
public:
    size_t get_tags_count();
    metadata_tag_ptr get_tag(size_t _Index);
private:
    virtual size_t get_tags_count_impl() = 0;
    virtual metadata_tag_ptr get_tag_impl(size_t _Index) = 0;
};

inline size_t metadata::Impl::get_tags_count()
{
    return get_tags_count_impl();
}

inline metadata_tag_ptr metadata::Impl::get_tag(size_t _Index)
{
    return get_tag_impl(_Index);
}


} // namespace openmedia 
#endif

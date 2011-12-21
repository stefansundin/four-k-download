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


/// \file  DTFFMetadata.h

#ifndef _DTFFMETADATA_H_INCLUDED_
#define _DTFFMETADATA_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <vector>

#include <openmedia/DTMetadata.h>
#include "../DTMetadataImpl.h"

struct AVMetadata;

namespace openmedia {

/// \class  ff_metadata_impl
class ff_metadata_impl: public metadata::Impl
{
public:
    ff_metadata_impl(AVMetadata * _AVMetadata);

private:
    virtual size_t get_tags_count_impl();
    virtual metadata_tag_ptr get_tag_impl(size_t _Index);

private:
    struct _Tag
    {
        _Tag(const std::string & _Key, const std::string & _Value): Key(_Key), Value(_Value){}
        std::string Key;    
        std::string Value;
    };

    std::vector<_Tag> m_Tags;
    
};

/// \class  ff_metadata
class ff_metadata: public metadata
{
public:
    ff_metadata(AVMetadata * _Metadata): metadata( new ff_metadata_impl(_Metadata))
    {}

};

inline size_t ff_metadata_impl::get_tags_count_impl()
{
    return m_Tags.size();
}

inline metadata_tag_ptr ff_metadata_impl::get_tag_impl(size_t _Index)
{
    return metadata_tag_ptr(new metadata_tag(m_Tags[_Index].Key, m_Tags[_Index].Value));
}


} // namespace openmedia 

#endif

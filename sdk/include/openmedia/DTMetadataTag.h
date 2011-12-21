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


/// \file   DTMetadataTag.h

#ifndef _DTMETADATATAG_H_INCLUDED_
#define _DTMETADATATAG_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <string>

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <openmedia/DTConfig.h>
#include <openmedia/DTDeclareImpl.h>
#include <openmedia/DTCommon.h>

namespace openmedia {

class metadata_tag;
typedef boost::shared_ptr<metadata_tag> metadata_tag_ptr;

/// \class  metadata_tag
class _OPENMEDIASDK_API metadata_tag: interface_base
{
    DT_DECLARE_IMPL2(metadata_tag);
public:
    std::string     get_key();
    std::string     get_value();
    virtual ~metadata_tag();

public:
    metadata_tag(const std::string & _Key, const std::string & _Value);

protected:
    metadata_tag(metadata_tag::Impl * _Impl);
    
private:
    metadata_tag::Impl * m_pImpl;
};

} // namespace openmedia 

#endif

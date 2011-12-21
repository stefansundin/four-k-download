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


/// \file   DTMetadata.h

#ifndef _DTMETADATA_H_INCLUDED_
#define _DTMETADATA_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <openmedia/DTDeclareImpl.h>
#include <openmedia/DTConfig.h>
#include <openmedia/DTCommon.h>
#include <openmedia/DTMetadataTag.h>

namespace openmedia {

class metadata;
typedef boost::shared_ptr<metadata> metadata_ptr;

/// \class  metadata
class _OPENMEDIASDK_API metadata : interface_base
{
    DT_DECLARE_IMPL2(metadata);
public:
    size_t              get_tags_count();
    metadata_tag_ptr    get_tag(size_t _Index);

    virtual ~metadata();

protected:
    metadata(metadata::Impl * _Impl);

private:
    metadata::Impl * m_pImpl;    
};

}
#endif

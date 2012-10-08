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


/// \file   DTMediaHandle.h

#ifndef _DTMEDIAHANDLE_H_INCLUDED_
#define _DTMEDIAHANDLE_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <openmedia/DTCommon.h>
#include <openmedia/DTDeclareImpl.h>

namespace openmedia {

class media_splitter;
typedef boost::shared_ptr<media_splitter> media_splitter_ptr;

class media_handle;
typedef boost::shared_ptr<media_handle> media_handle_ptr;

/// \class  media_handle
class _OPENMEDIASDK_API media_handle : interface_base
{
    DT_DECLARE_IMPL2(media_handle);
public:
    media_splitter_ptr get_media_splitter();
    const media_splitter_ptr get_media_splitter() const;
    virtual ~media_handle();

    static media_handle_ptr create(media_splitter_ptr & _MediaSplitter);

protected:
    media_handle(Impl * _Impl);

private:
    Impl * m_pImpl;
};

} // namespace openmedia

#endif

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


#include "DTHeadersMedia.h"
#include "DTMediaHandle.h"
#include <openmedia/DTMediaSplitter.h>

#include "DTMediaHandleImpl.h"

namespace openmedia {

media_splitter_ptr media_handle::get_media_splitter()
{
    return impl()->get_media_splitter();
}

const media_splitter_ptr media_handle::get_media_splitter() const
{
    return impl()->get_media_splitter();
}

media_handle::media_handle(media_handle::Impl * _Impl) : m_pImpl(_Impl)
{
    DT_STRONG_ASSERT(NULL != _Impl);
}

media_handle::~media_handle()
{
    delete m_pImpl;
}

class media_handle_impl : public media_handle::Impl
{
public:
    media_handle_impl(media_splitter_ptr & _MediaSplitter) : m_MediaSplitter(_MediaSplitter)
    {}

private:
    virtual const media_splitter_ptr get_media_splitter() const
    {
        return m_MediaSplitter;
    }

    virtual media_splitter_ptr get_media_splitter()
    {
        return m_MediaSplitter;
    }

private:
    media_splitter_ptr m_MediaSplitter;

};

class media_handle_ : public media_handle
{
public:
    media_handle_(media_splitter_ptr & _MediaSplitter) : media_handle( new media_handle_impl(_MediaSplitter) )
    {
    }
};

media_handle_ptr media_handle::create(media_splitter_ptr & _MediaSplitter)
{
    return media_handle_ptr( new media_handle_(_MediaSplitter) );
}

}

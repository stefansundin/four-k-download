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


#ifndef _DTCOMMON_H_INCLUDED_
#define _DTCOMMON_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/noncopyable.hpp>

#include <openmedia/DTConfig.h>

#define DT_IF_DISABLE_EXCEPTIONS(A)

#define DT_SIMPLE_PROPERTY(TYPE, NAME) private:\
    TYPE m_##NAME;\
    public:\
    void set_##NAME(TYPE _##NAME) {m_##NAME = _##NAME;}\
    public:\
    TYPE get_##NAME() const {return m_##NAME;}

namespace openmedia {

class impl_base: boost::noncopyable
{
public:
    virtual ~impl_base() = 0;
};

class _OPENMEDIASDK_API interface_base
{
protected:
    interface_base() {}
    ~interface_base() {}
private:
    interface_base(const interface_base &);
    interface_base & operator=(const interface_base &);
};

namespace utils {

class no_delete
{
public:
    template <class T>
    void operator () (const T * const)
    {
    }
};

template <class FreeBufferFunc>
class func_deleter
{
private:
    FreeBufferFunc m_FreeBuffer;

public:
    func_deleter(FreeBufferFunc _FreeBuffer) : m_FreeBuffer(_FreeBuffer){};
    template <typename T>
    void operator ()(T * _Buffer)
    {
        if (m_FreeBuffer)
            m_FreeBuffer(&_Buffer);
    }
};

}

}

#endif

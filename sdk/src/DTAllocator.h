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


/// \file   DTCommonPacket.h

#ifndef _DTALLOCATOR_H_INCLUDED_
#define _DTALLOCATOR_H_INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/shared_array.hpp>
#include <openmedia/DTCommon.h>

namespace openmedia { namespace Allocator {

/// \class  DefaultBufferAllocator
template <class T>
class DefaultBufferAllocator
{
public:
    static T *    AllocBuffer(size_t _Count);
    static void   FreeBuffer(T ** _Buffer);
};

template <class T>
inline T * DefaultBufferAllocator<T>::AllocBuffer(size_t _Count)
{
    T * buffer = (uint8_t *)malloc( _Count * sizeof(T) );
    if (!buffer)
        BOOST_THROW_EXCEPTION( std::bad_alloc() );
    return buffer;
}

template <class T>
inline void DefaultBufferAllocator<T>::FreeBuffer(T ** _Buffer)
{
    if (_Buffer)
    {
        free(*_Buffer);
        *_Buffer = NULL;
    }
}

/// \class  DefaultArrayAllocator
template <class T, template <class> class BufferAllocator>
class DefaultArrayAllocator
{
private:
    class Deleter
    {
    public:
        void operator () (T * _Array);
    };
public:
    static T *  AllocBuffer(size_t _Count);
    static void FreeBuffer(T ** _Buffer);

    static boost::shared_array<T> AllocArray(size_t _Count);
};


template <class T, template <class> class BufferAllocator>
inline void DefaultArrayAllocator<T, BufferAllocator>::Deleter::operator () (T * _Array)
{
    BufferAllocator<T>::FreeBuffer(&_Array);
}

template <class T, template <class> class BufferAllocator>
inline T *  DefaultArrayAllocator<T, BufferAllocator>::AllocBuffer(size_t _Count)
{
    return BufferAllocator<T>::AllocBuffer(_Count);
}

template <class T, template <class> class BufferAllocator>
inline void DefaultArrayAllocator<T, BufferAllocator>::FreeBuffer(T ** _Buffer)
{
    return BufferAllocator<T>::FreeBuffer(_Buffer);
}

template <class T, template <class> class BufferAllocator>
inline boost::shared_array<T> DefaultArrayAllocator<T, BufferAllocator>::AllocArray(size_t _Count)
{
    T * buffer = BufferAllocator<T>::AllocBuffer(_Count);
    return boost::shared_array<T>( buffer, Deleter() );
}

} } // namespace openmedia::Allocator

#endif // #ifndef _DTALLOCATOR_H_INCLUDED_

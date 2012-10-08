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


/// \file   DTMemoryBuffer.h

#ifndef _DTMEMORYBUFFER_H_INCLUDED_
#define _DTMEMORYBUFFER_H_INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <openmedia/DTCommon.h>

#include <boost/noncopyable.hpp>
#include <boost/foreach.hpp>
#include <list>
#include <algorithm>

namespace openmedia {
namespace details {

class memory_buffer_simple: boost::noncopyable
{
public:
    memory_buffer_simple();
    void    push_back(const uint8_t * _Buffer, size_t _SizeBytes);
    void    pop_front(size_t _SizeBytes, uint8_t * _Buffer, size_t & _OutBytesReceived); ///< output buffer must be created by user
    size_t  size() const;
    size_t  buffers_count() const; ///< debug
    ~memory_buffer_simple();

private:
    struct data
    {
    public:
        data();
        ~data() throw();

        uint8_t *   buffer;
        uint8_t *   buffer_pos;
        size_t      size;

        data(const data & _data) throw();
        data & operator=(const data & _data) throw();

    };

private:
    typedef std::list<data> DataList;

    size_t      m_DataSizeBytes;
    DataList    m_DataList;
};

inline size_t memory_buffer_simple::size() const
{
    return m_DataSizeBytes;
}

inline size_t  memory_buffer_simple::buffers_count() const
{
    return m_DataList.size();
}

inline memory_buffer_simple::memory_buffer_simple() : m_DataSizeBytes(0)
{
}

inline void memory_buffer_simple::push_back(const uint8_t * _Buffer, size_t _SizeBytes)
{
    data newData;
    newData.buffer = static_cast<uint8_t*>(malloc(_SizeBytes));
    DT_ASSERT(NULL != newData.buffer);
    if (!newData.buffer)
        throw std::bad_alloc();

    memcpy(newData.buffer, _Buffer, _SizeBytes);
    newData.size = _SizeBytes;
    newData.buffer_pos = newData.buffer;

    m_DataList.push_back( newData );    
    m_DataSizeBytes += _SizeBytes;
}

inline void memory_buffer_simple::pop_front(size_t _SizeBytes, uint8_t * _Buffer, size_t & _OutBytesReceived)
{
    DT_ASSERT(NULL != _Buffer);
    size_t sizeOut = 0;
    size_t bytesNeed = _SizeBytes;
    uint8_t * posToWrite = _Buffer;
    _OutBytesReceived = 0;

    while (sizeOut < _SizeBytes && m_DataSizeBytes > 0)
    {
        DT_ASSERT(m_DataList.size() != 0);

        DataList::reference front = m_DataList.front();
        const size_t bytesCopyFromCurrent = (std::min)(bytesNeed, front.size);
        memcpy(posToWrite, front.buffer_pos, bytesCopyFromCurrent);
        front.size -= bytesCopyFromCurrent;
        front.buffer_pos += bytesCopyFromCurrent;
        if (0 == front.size)
        {
            free(static_cast<void*>(front.buffer));
            m_DataList.pop_front();
        }
        m_DataSizeBytes -= bytesCopyFromCurrent;

        sizeOut     += bytesCopyFromCurrent;
        posToWrite  += bytesCopyFromCurrent;
        bytesNeed   -= bytesCopyFromCurrent;
        
    }

    _OutBytesReceived = sizeOut;
}

inline memory_buffer_simple::~memory_buffer_simple()
{
    BOOST_FOREACH(data & d, m_DataList)
    {
        free(static_cast<void*>(d.buffer));
        d.buffer = NULL;
        d.buffer_pos = NULL;
        d.size = 0;
    }
}

inline memory_buffer_simple::data::data() : buffer(NULL), buffer_pos(0), size(0)
{
}

inline memory_buffer_simple::data::data(const memory_buffer_simple::data & _data) throw() : 
buffer(_data.buffer),
buffer_pos(_data.buffer_pos),
size(_data.size)
{
}

inline memory_buffer_simple::data & memory_buffer_simple::data::operator=(const memory_buffer_simple::data & _data) throw()
{
    memory_buffer_simple::data newData(_data);
    std::swap(newData, *this);
    return *this;
}

inline memory_buffer_simple::data::~data() throw()
{
}

} // namespace details
} // namespace openmedia

#endif // #ifndef _DTMEMORYBUFFER_H_INCLUDED_

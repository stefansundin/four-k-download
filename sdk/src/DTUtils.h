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


/// \file   DTUtils.h

#ifndef _DTUTILS_H_INCLUDED_
#define _DTUTILS_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <cstdlib>
#include <exception>
#include <openmedia/DTCStdInt.h>

namespace openmedia { namespace utils {

/// \todo TODO 
class fixed_memory_buffer : boost::noncopyable
{
public:
    fixed_memory_buffer() : m_Buffer(NULL),
        m_Size(0)
    {}

    uint8_t * get_buffer(size_t _SizeBytes)
    {
        if (_SizeBytes > m_Size)
        {
            m_Buffer = (uint8_t*)realloc(m_Buffer, _SizeBytes);
            if (!m_Buffer)
            {
                BOOST_THROW_EXCEPTION(std::bad_alloc());
            }
            m_Size = _SizeBytes;
        }
        return m_Buffer;
    };

    ~fixed_memory_buffer()
    {
        free(m_Buffer);
    }

private:
    uint8_t * m_Buffer;
    size_t m_Size;

};

inline errno_t dt_fopen(FILE ** _File, const char * _Filename, const char * _Mode)
{
#ifdef _MSC_VER
    return  fopen_s(_File, _Filename, _Mode);
#else
    if (NULL == _File)
        return EINVAL;
    *_File = fopen(_Filename, _Mode);
    if (_File)
        return 0;
    else
        return errno;
#endif
}


#ifdef _WIN32
inline errno_t dt_wopen(int * _FileHandle, const wchar_t * _Filename, int _OpenFlag, int _PermissionFlag)
{
#ifdef _MSC_VER
    //return  _wsopen_s(_FileHandle, _Filename, _OpenFlag, _SH_DENYRW, _PermissionFlag);
    if (NULL == _FileHandle)
        return EINVAL;
    *_FileHandle = _wopen(_Filename, _OpenFlag, _PermissionFlag);
    if (_FileHandle)
        return 0;
    else
        return errno;

#else
    if (NULL == _FileHandle)
        return EINVAL;
    *_FileHandle = _wopen(_Filename, _OpenFlag, _PermissionFlag);
    if (_FileHandle)
        return 0;
    else
        return errno;
#endif
}
#endif

} } // namespace openmedia::utils

#endif
	
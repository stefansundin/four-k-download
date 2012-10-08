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


#include "DTHeadersCommon.h"
#include "DTFOpen.h"
#include <openmedia/DTString.h>

namespace openmedia {

namespace {
struct fclose__
{
    void operator()(FILE * f)
    {
        if (f)
            fclose(f);
    }
};
}

FILEPtr dt_fopen(const std::wstring & fileNameUtf16, const wchar_t * attr)
{
#ifndef _WIN32
    FILEPtr f = boost::shared_ptr<FILE>( 
        fopen( utf16_to_utf8(fileNameUtf16).c_str(), utf16_to_utf8(std::wstring(attr)).c_str() ), fclose__()
        );
#else
    FILE * f_ = _wfopen( fileNameUtf16.c_str(), attr );
    FILEPtr f = boost::shared_ptr<FILE>(f_, fclose__());
#endif
    return f;
}

FILEPtr dt_fopen(const std::string & fileNameUtf8, const char * attr)
{
#ifndef _WIN32
    FILEPtr f = boost::shared_ptr<FILE>( fopen(fileNameUtf8.c_str(), attr), fclose__());
#else
    FILE * f_ = _wfopen( utf8_to_utf16(fileNameUtf8).c_str(), utf8_to_utf16(std::string(attr)).c_str() );
    FILEPtr f = boost::shared_ptr<FILE>(f_, fclose__());
#endif
    return f;
}

}
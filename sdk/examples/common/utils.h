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


#ifndef _EXAMPLE_COMMON_UTILS_H_
#define _EXAMPLE_COMMON_UTILS_H_

inline std::string utf16_to_ansi(const std::wstring & Utf16String)
{
    char * currentLocale = ::setlocale(LC_ALL, NULL);
    std::string currentLocaleStr = currentLocale;
    ::setlocale(LC_ALL, "");

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4996)
#endif
    size_t size = wcstombs(NULL, Utf16String.c_str(), 0);
    std::string ansi;
    ansi.resize(size);
    size = wcstombs(&ansi[0], Utf16String.c_str(), ansi.length());
    ::setlocale(LC_ALL, currentLocaleStr.c_str());

#ifdef _MSC_VER
#pragma warning(pop)
#endif


    return ansi;
}

#endif
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


/// \file   DTString.h

#ifndef _DTSTRING_H_INCLUDED_
#define _DTSTRING_H_INCLUDED_

#include <openmedia/DTCommon.h>
#include <string>

namespace openmedia {

std::string     _OPENMEDIASDK_API ansi_to_utf8(const std::string & _AnsiString);
std::wstring    _OPENMEDIASDK_API utf8_to_utf16(const std::string & _Utf8String);
std::string     _OPENMEDIASDK_API utf16_to_utf8(const std::wstring & _Utf16String);

} // namespace openmedia

#endif


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


#ifndef _DT_FOPEN_H_
#define _DT_FOPEN_H_

#include <stdio.h>
#include <boost/shared_ptr.hpp>

namespace openmedia {

typedef boost::shared_ptr<FILE> FILEPtr;

FILEPtr dt_fopen(const std::string & fileNameUtf8, const char * attr);
FILEPtr dt_fopen(const std::wstring & fileNameUtf16, const wchar_t * attr);

} // namespace openmedia

#endif

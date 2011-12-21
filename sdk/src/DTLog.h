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


#ifndef _DT_LOG_1_INCLUDED_
#define _DT_LOG_1_INCLUDED_

#include <openmedia/DTConfig.h>

#if 0

#include <iostream>
#define DT_LOG(A) ::std::cerr

#else

class empty_log 
{
public:
    template <class T>
    empty_log & operator << (const T &) { return *this; }
};

#define DT_LOG(A) empty_log()

#endif


#endif

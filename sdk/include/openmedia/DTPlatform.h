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


/// \file   DTPlatform.h

#ifndef _DTPALTFORM_H_INCLUDED_
#define _DTPALTFORM_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifdef _MSC_VER

#include <cerrno>

#define dt_snprintf     _snprintf_s
#define dt_fprintf      fprintf_s

#define PLATFORM_ASSERTE(EXPR) _ASSERTE(EXPR)
#define PLATFORM_ALL_ASSERTE(EXPR) _ASSERTE(EXPR)

#else

#include <cassert>
#include <stdint.h>

#define dt_snprintf     snprintf
#define dt_fprintf      fprintf

#ifndef _ERRCODE_DEFINED
#define _ERRCODE_DEFINED
typedef int errno_t;
#endif

#ifdef _DEBUG
#   define PLATFORM_ASSERTE(EXPR) assert(EXPR)
#   define PLATFORM_ALL_ASSERTE(EXPR)
#else
#   define PLATFORM_ASSERTE(EXPR)
#   define PLATFORM_ALL_ASSERTE(EXPR)
#endif

#endif

#endif

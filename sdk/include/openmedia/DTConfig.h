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


/// \file   DTConfig.h

#ifndef _DTCONFIG_H_INCLUDED_
#define _DTCONFIG_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifdef  OPENMEDIASDK_EXPORTS
#define _OPENMEDIASDK_API __declspec(dllexport)
#elif defined(_USE_OPENMEDIASDK_DLL)
#define _OPENMEDIASDK_API __declspec(dllimport)
#else
#define _OPENMEDIASDK_API
#endif

#ifdef _DEBUG
#define DT_DEBUG 
#else
#define DT_RELEASE
#endif

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#define DT_CONFIG_HAVE_UTF16_OPEN 1
#else
#define DT_CONFIG_HAVE_UTF16_OPEN 0
#endif

#define DT_HAVE_PYTHON_SCRIPT 0
#define DT_HAVE_QT_SCRIPT 1

#endif // #ifndef _DTCONFIG_H_INCLUDED_

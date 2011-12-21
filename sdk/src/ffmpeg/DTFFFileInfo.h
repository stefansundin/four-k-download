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


/// \file   DTFFFileInfo.h

#ifndef _DTFFFILEINFO_H_INCLUDED_
#define _DTFFFILEINFO_H_INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/shared_ptr.hpp>

#include <openmedia/DTCommon.h>
#include <openmedia/DTFileInfo.h>

struct AVFormatContext;

namespace openmedia {

class ff_file_info_general;
typedef boost::shared_ptr<ff_file_info_general> ff_file_info_general_ptr;

/// \class  ff_file_info_general
class ff_file_info_general : public file_info_general
{
public:
    ff_file_info_general(AVFormatContext * _Formatcontext);
    void set(AVFormatContext * _Formatcontext);
}; 

} // namespace openmedia

#endif // #ifndef _DTFFFILEINFO_H_INCLUDED_

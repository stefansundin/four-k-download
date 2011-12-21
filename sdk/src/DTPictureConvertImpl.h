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


/// \file   DTPictureConvertImpl.h

#ifndef _DTPICTURECONVERTIMPL_H_INCLUDED_
#define _DTPICTURECONVERTIMPL_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <openmedia/DTCommon.h>
#include <openmedia/DTPictureConvert.h>

namespace openmedia {

/// \class  picture_convert::Impl
class picture_convert::Impl : public impl_base
{
public:
    virtual video_data_ptr convert(video_data_ptr _InputData) = 0;
};

}

#endif // #ifndef _DTPICTURECONVERTIMPL_H_INCLUDED_

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


#ifndef _OPENMEDIA_DT_MEDIA_SOURCE_H_INCLUDED_1CD79B49
#define _OPENMEDIA_DT_MEDIA_SOURCE_H_INCLUDED_1CD79B49

#ifdef _MSC_VER
#pragma once
#endif

namespace openmedia {

class media_source
{
public:
    virtual double get_duration() const = 0;
    virtual double get_current_pos() const = 0;
    virtual double skip(double duration) = 0;
    virtual bool enabled() const = 0;
    virtual void enabled(bool val) = 0;

    virtual ~media_source() = 0;
};

inline media_source::~media_source()
{}

} // namespace openmedia

#endif

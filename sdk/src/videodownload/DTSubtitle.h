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


#ifndef _OPENMEDIA_DT_SUBTITLE_H_INCLUDED_7125B0E7
#define _OPENMEDIA_DT_SUBTITLE_H_INCLUDED_7125B0E7

#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>

namespace openmedia {
namespace downloader {

class subtitles;
typedef boost::shared_ptr<subtitles> subtitles_ptr;

class subtitles
{
public:
    ~subtitles();

public:
    size_t      count();
    std::string id_at(size_t index);
    std::string lang_at(size_t index);
    void        save(size_t index, const std::string & fileNameUtf8);

private:
    void        save_impl(size_t index, const std::string & fileNameUtf8);

public:
    class Impl;
private:
    boost::scoped_ptr<Impl> impl_;
protected:
    subtitles(Impl * _impl);
};


}
}

#endif

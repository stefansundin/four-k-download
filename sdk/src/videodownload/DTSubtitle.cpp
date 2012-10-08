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


#include "DTHeadersVideoDownload.h"
#include "DTSubtitle.h"
#include "DTSubtitleDownload.h"
#include "DTSubtitleImpl.h"
#include "../DTFOpen.h"
#include <sstream>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

namespace openmedia {
namespace downloader {


subtitles::~subtitles()
{}

subtitles::subtitles(Impl * _impl): impl_(_impl)
{}

size_t subtitles::count()
{
    return impl_->tracks().size();    
}

std::string subtitles::id_at(size_t index)
{
    return impl_->tracks().at(index).lang_code;
}

std::string subtitles::lang_at(size_t index)
{
    return impl_->tracks().at(index).lang_translated;
}

void subtitles::save(size_t index, const std::string & fileNameUtf8)
{
    boost::thread(boost::bind(&subtitles::save_impl, this, index, fileNameUtf8));
}

void subtitles::save_impl(size_t index, const std::string & fileNameUtf8)
{
    std::vector<transcript_element> subs;
    impl_->download_subtitle(index, subs);
    
    std::stringstream strm;
    subtitles_to_srt(subs, strm);

    FILEPtr f = dt_fopen(fileNameUtf8, "wb");
    if (f)
    {
        static const char bom[] = {0xEF, 0xBB, 0xBF};
        fwrite(bom, sizeof(bom), 1, f.get());
        fwrite(strm.str().c_str(), strm.str().size(), 1, f.get());
    }
}

}
}

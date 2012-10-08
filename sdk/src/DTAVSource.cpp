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



// precompiled header begin
#include "DTHeadersMedia.h"
// precompiled header end

#include <boost/make_shared.hpp>
#include "DTAVSourceImpl.h"
#include "DTAVSourceFile.h"

namespace openmedia {

video_source * av_source::video()
{
    return impl()->video();
}

audio_source * av_source::audio()
{
    return impl()->audio();
}

double av_source::seek(double position)
{
    return impl()->seek(position);
}

void av_source::apply_settings(media_settings_ptr settings)
{
    return impl()->apply_settings(settings);
}

av_source::~av_source() 
{
    delete m_pImpl;
}

av_source::av_source(Impl * _Impl) : m_pImpl (_Impl)
{}

av_source_ptr av_source_creator::create(const char * fileName)
{
    return boost::make_shared<av_video_file>(fileName);
}

}

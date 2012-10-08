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


#ifndef _OPENMEDIA_DT_AV_SOURCE_H_INCLUDED_42A804D9
#define _OPENMEDIA_DT_AV_SOURCE_H_INCLUDED_42A804D9

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <openmedia/DTCommon.h>
#include <openmedia/DTDeclareImpl.h>
#include <openmedia/DTTypes.h>

namespace openmedia {

class media_settings;
typedef boost::shared_ptr<media_settings> media_settings_ptr;

class video_source;
class audio_source;

class av_source;
typedef boost::shared_ptr<av_source> av_source_ptr;

/// \class  av_source
class _OPENMEDIASDK_API av_source : interface_base
{
    DT_DECLARE_IMPL2(av_source);
public:
    video_source * video();
    audio_source * audio();
    double seek(double positionMillisec);
    void apply_settings(media_settings_ptr settings);

    virtual ~av_source();

protected:
    av_source(Impl * _Impl);

private:
    Impl * m_pImpl;
};

class _OPENMEDIASDK_API av_source_creator 
{
public:
    static av_source_ptr create(const char * fileName);
};

}

#endif

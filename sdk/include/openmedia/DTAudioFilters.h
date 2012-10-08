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


#ifndef _OPENMEDIA_DT_AUDIO_FILTERS_H_INCLUDED_4B4D92CF
#define _OPENMEDIA_DT_AUDIO_FILTERS_H_INCLUDED_4B4D92CF

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <openmedia/DTDeclareImpl.h>
#include <openmedia/DTConfig.h>
#include <openmedia/DTCommon.h>
#include <openmedia/DTAudioBuffer.h>
#include <openmedia/DTMediaSettings.h>

namespace openmedia {

class audio_data;
typedef boost::shared_ptr<audio_data> audio_data_ptr;

class audio_format;
typedef boost::shared_ptr<audio_format> audio_format_ptr;

class audio_filter;
typedef boost::shared_ptr<audio_filter> audio_filter_ptr;

class _OPENMEDIASDK_API audio_filter : public audio_buffer
{
    DT_DECLARE_IMPL2(audio_filter);
public:
    void apply(media_settings * settings);
protected:
    audio_filter(audio_filter::Impl * _Impl);

private:
    Impl * m_pImpl;
};

class _OPENMEDIASDK_API audio_filters_utils
{
public:    
    static audio_filter_ptr create_amplify(const audio_format * audioFormat);

};

}

#endif

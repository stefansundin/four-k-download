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


#ifndef _DT_VIDEO2MP3_H_
#define _DT_VIDEO2MP3_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <boost/function.hpp>

#include <openmedia/DTConfig.h>

namespace openmedia {

class media_handle;
typedef boost::shared_ptr<media_handle> media_handle_ptr;

namespace audio {

class video2mp3_control;
typedef boost::shared_ptr<video2mp3_control> video2mp3_control_ptr;
class _OPENMEDIASDK_API video2mp3_control
{
public:
    virtual ~video2mp3_control(){}
    virtual void pause() = 0;
    virtual void resume() = 0;
    virtual void cancel() = 0;
};

class _OPENMEDIASDK_API video2mp3 
{
public:
    enum State
    {
        stateProgress = 0,
        stateFinish = 1,
        stateError,
        statePause,
        stateResume
    };

    enum Operation
    {
        opNormal = 0,
        opCancel = 1
    };

    typedef boost::function< bool (State, double) > ConvertStateNotify;
    typedef boost::function< void (video2mp3_control_ptr) > GetControl;

    static void convert(
        std::string InputFileName,
        std::string OutputMp3Name,
        std::wstring Title,
        ConvertStateNotify StateNotify,
        bool UseSourceBitrate);

    static void convert(
        media_handle_ptr MediaHandle,
        std::string OutputMp3Name,
        std::wstring Title,
        ConvertStateNotify StateNotify,
        bool UseSourceBitrate);

    static void convert(media_handle_ptr MediaHandle,
        std::string OutputMp3Name,
        std::wstring Title,
        ConvertStateNotify StateNotify,
        GetControl OnGetControl,
        bool UseSourceBitrate);
};

 
} } // namespace openmedia::audio

#endif // #ifndef _DT_VIDEO2MP3_H_

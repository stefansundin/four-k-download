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

namespace openmedia { namespace audio { 

class _OPENMEDIASDK_API video2mp3 
{
public:
    enum State
    {
        stateProgress = 0,
        stateFinish = 1,
        stateError
    };

    enum Operation
    {
        opNormal = 0,
        opCancel = 1
    };

    typedef boost::function< bool (State, double) > ConvertStateNotify;

    static void convert(std::wstring InputFileName, std::wstring OutputMp3Name, std::wstring Title, ConvertStateNotify StateNotify);
    static void convert(std::string InputFileName, std::string OutputMp3Name, std::wstring Title, ConvertStateNotify StateNotify);
    
};

 
} } // namespace openmedia::audio

#endif // #ifndef _DT_VIDEO2MP3_H_

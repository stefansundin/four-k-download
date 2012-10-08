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


/// \file   video2mp3.cpp

#define BOOST_THREAD_USE_LIB

#include <openmedia/DTVideo2Mp3.h>
#include <iostream>

#ifdef _MSC_VER
#   pragma comment(lib, "dtcommonsdk-static.lib")
#   pragma comment(lib, "dtmediasdk-static.lib")
#   pragma comment(lib, "avcodec.lib")
#   pragma comment(lib, "avformat.lib")
#   pragma comment(lib, "avutil.lib")
#   pragma comment(lib, "swresample.lib")
#   pragma comment(lib, "swscale.lib")
#   pragma comment(lib, "libmp3lame.dll.a")
#endif

using namespace openmedia;
using namespace std;

bool CallbackFun(audio::video2mp3::State state, double progress)
{
    cout << state << " " << progress << "\n";
    return true;
}

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        cout << "usage: " << argv[0] << " [inputfile] [outputfile.mp3]\n";
        return 1;
    }
    
    wstring title = L"Title";
    audio::video2mp3::convert(argv[1], argv[2], title, &CallbackFun, true);
    
    char ch;
    cin >> ch;

	return 0;
}


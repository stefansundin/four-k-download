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


#define BOOST_THREAD_USE_LIB

#include <openmedia/DTVideo2Mp3.h>
#include <iostream>

#ifdef _MSC_VER
#   pragma comment(lib, "dtcommonsdk-static.lib")
#   pragma comment(lib, "dtmediasdk-static.lib")

#   pragma comment(lib, "libavformat.a")
#   pragma comment(lib, "libavcodec.a")
#   pragma comment(lib, "libavutil.a")
#   pragma comment(lib, "libgcc.lib")
#   pragma comment(lib, "libmingwex.lib") 
#   pragma comment(lib, "libmp3lame.a") 

#endif

bool CallbackFun(openmedia::audio::video2mp3::State state, double progress)
{
    std::cout << state << " " << progress << "\n";
    return true;
}

#if defined(_MSC_VER) && defined(_UNICODE)
int wmain(int argc, wchar_t * argv[])
#else
int main(int argc, char* argv[])
#endif
{
    if (argc < 3)
    {
        std::cerr << "usage: video2mp3.exe [INPUT] [MP3]";
        return 1;
    }
    
    std::wstring title = L"Title";
    openmedia::audio::video2mp3::convert(argv[1], argv[2], title, &CallbackFun);
	return 0;
}


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

#include <openmedia/DTMediaDownloader.h>
#include <openmedia/DTMediaSite.h>
#include <boost/thread.hpp>
#include <iostream>

#include "../common/utils.h"


#ifdef _MSC_VER
#   pragma comment(lib, "dtdownloadsdk-static.lib")
#   pragma comment(lib, "dtcommonsdk-static.lib")
#   pragma comment(lib, "liburdl.lib")
#endif

boost::mutex outputInfoMutex;

void OpenCallback(openmedia::downloader::url_parser_result_ptr result, openmedia::downloader::url_parser::ErrorCode errorCode)
{
    if (!result)
        return;

    for (size_t i = 0; i < result->size(); ++i)
    {
        openmedia::downloader::media_download_list_ptr downloadList = result->at(i);
        if (!downloadList || !downloadList->size())
            continue;

        boost::mutex::scoped_lock lock(outputInfoMutex);
        std::cout << "\n\nRESULT:";

#ifdef _MSC_VER
        std::wcout << "title:\t" << downloadList->title() << "\n";
#endif
        std::cout << "title:\t" << downloadList->title_utf8() << "\n";
        std::cout << "jpeg size:\t" << downloadList->thumbnail()->size() << "\n";
        std::cout << "duration (sec): " << downloadList->duration() << "\n";

        for (size_t i = 0; i < downloadList->size(); ++i)
        {
            openmedia::downloader::media_download_info mediaInfo = downloadList->media(i);
            std::cout << "\ntype:\t" << mediaInfo.content_type() << "\n";  
            std::cout << "size:\t" << mediaInfo.content_size() << " bytes\n";

            std::cout << "resolution:\t" << mediaInfo.width() << "x" << mediaInfo.height() << "\n";
        }    
    }
}

#if defined(_MSC_VER) && defined(_UNICODE)
int wmain(int argc, wchar_t * argv[])
#else
int main(int argc, char* argv[])
#endif
{
    if (argc < 2)
    {
        std::cerr << "usage: get_download_info.exe [URL] [URL] ...";
        return 1;
    }

    for (int i = 1; i < argc; ++i)
    {
    std::string url 
#if defined(_MSC_VER) && defined(_UNICODE)
        = utf16_to_ansi(argv[i]);
#else
        = argv[i];
#endif

        openmedia::downloader::media_site_type_t siteType = openmedia::downloader::media_site_utils::validate_url(url);

        if ( openmedia::downloader::mediaSiteUnknown == siteType)
        {
            std::cerr << "unknown site\n";
            continue;
        }   

        std::cout << "Site type: " << siteType << "\n";

        openmedia::downloader::url_parser::parse_url_async(url, openmedia::downloader::url_parser::parseNormal, &OpenCallback);
    }

	return 0;
}


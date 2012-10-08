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


/// \file   get_download_info_async.cpp
#define BOOST_THREAD_USE_LIB

#include <openmedia/DTMediaDownloader.h>
#include <openmedia/DTMediaSite.h>
#include <boost/thread.hpp>
#include <iostream>

#include "../common/utils.h"

#ifdef _MSC_VER
#   include <conio.h>
#endif

#ifdef _MSC_VER
#   pragma comment(lib, "dtvideodownloadsdk-static.lib")
#   pragma comment(lib, "dtdownloadsdk-static.lib")
#   pragma comment(lib, "dtcommonsdk-static.lib")
#   pragma comment(lib, "liburdl.lib")
#endif

using namespace std;
using namespace boost;
using namespace openmedia::downloader;

mutex outputInfoMutex;

void OpenCallback(url_parser_result_ptr result, url_parser::ErrorCode errorCode)
{
    if (!result)
        return;

    for (size_t i = 0; i < result->size(); ++i)
    {
        media_download_list_ptr downloadList = result->at(i);
        if (!downloadList || !downloadList->size())
            continue;

        mutex::scoped_lock lock(outputInfoMutex);
        cout << "\n\nRESULT:";
        cout << "title:\t" << downloadList->title_utf8() << "\n";
        cout << "jpeg size:\t" << downloadList->thumbnail()->size() << "\n";
        cout << "duration (sec): " << downloadList->duration() << "\n";

        for (size_t i = 0; i < downloadList->size(); ++i)
        {
            media_download_info mediaInfo = downloadList->media(i);
            cout << "\ntype:\t" << mediaInfo.content_type() << "\n";  
            cout << "size:\t" << mediaInfo.content_size() << " bytes\n";
        }    
    }
}

int main(int argc, char* argv[])
{
    std::string defaultUrl = "http://www.youtube.com/watch?v=N0m1XmvBey8";

    for (int i = 1; i < (argc < 2 ? 2 : argc ); ++i)
    {
        string url = (argc < 2 ? defaultUrl : argv[i]);
        media_site_type_t siteType = media_site_utils::validate_url(url);

        if (mediaSiteUnknown == siteType 
            || mediaSiteNull == siteType)
        {
            cerr << "unknown site\n";
            continue;
        }   

        cout << "Site type: " << siteType << "\n";
        url_parser::parse_url_async(url, url_parser::parseNormal, &OpenCallback);
    }
    char ch;
    cin >> ch;
	return 0;
}


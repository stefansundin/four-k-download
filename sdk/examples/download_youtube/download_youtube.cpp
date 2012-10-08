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


/// \file   download_youtube.cpp

#define BOOST_THREAD_USE_LIB

#include <openmedia/DTHttpDownloader.h>
#include <openmedia/DTMediaDownloader.h>
#include <openmedia/DTMediaSite.h>
#include <boost/thread.hpp>
#include <iostream>
#include <boost/lexical_cast.hpp>

#include "../common/utils.h"

#ifdef _MSC_VER
#   pragma comment(lib, "dtvideodownloadsdk-static.lib")
#   pragma comment(lib, "dtdownloadsdk-static.lib")
#   pragma comment(lib, "dtcommonsdk-static.lib")

#   pragma comment(lib, "liburdl.lib")
#endif

using namespace std;
using namespace boost;
using namespace openmedia::downloader;

string destDir;

template <class T>
T & operator << (T & _Ostream, http_downloader::download_state_t _State)
{
    switch(_State)
    {
    case http_downloader::stateStart:  _Ostream << "[start]"; break;
    case http_downloader::stateCancel: _Ostream << "[cancel]"; break;
    case http_downloader::stateDownload: _Ostream << "[download]"; break;
    case http_downloader::stateError: _Ostream << "[error]"; break;
    case http_downloader::stateFinish: _Ostream << "[finish]"; break;
    case http_downloader::statePause: _Ostream << "[pause]"; break;
    case http_downloader::stateResume: _Ostream << "[resume]"; break;
    default: _Ostream << "[unknown]"; break;
    }
    return _Ostream;
}

mutex outputInfoMutex;

class Downloader
{
public:
    Downloader( media_url_handle_ptr DownloadHandle, 
        media_info_handle_ptr DownloadMediaInfo,
        const string & OutputFileName,
        const string & DownloadName,
        mutex & Guard) : mutex_(Guard),
        name_(DownloadName)

    {
        init(DownloadHandle, DownloadMediaInfo, OutputFileName);
    }

    void cancel()
    {
        media_downloader_->cancel();      
    }

    void pause()
    {
        media_downloader_->pause();        
    }

    void resume()
    {
        media_downloader_->resume();        
    }

private:
    void init( media_url_handle_ptr DownloadHandle, 
        media_info_handle_ptr DownloadMediaInfo,
        const string & OutputFileName)
    {
        media_downloader_.reset(
            new media_downloader(
            DownloadHandle,
            DownloadMediaInfo,
            OutputFileName,
            bind(&Downloader::StateNotify, this, _1), 
            bind(&Downloader::ProgressNotify, this, _1) )
            );
    }

private:
    std::string name_;
    boost::scoped_ptr< media_downloader > media_downloader_;

    void ProgressNotify(boost::uint64_t Bytes)
    {
        mutex::scoped_lock lock(mutex_);
        cout << "progress: " << name_ << " " << Bytes << "\n";
    }

    void StateNotify(media_downloader::download_state_t State)
    {
        mutex::scoped_lock lock(mutex_);
        cout << "state: " << name_ << " " << State << "\n";
    }

    mutable mutex & mutex_;

};

size_t filesCount = 0;
vector< shared_ptr< Downloader > > downloaders;

#define DOWNLOAD_MAX_VIDEO

void OpenCallback(url_parser_result_ptr result, url_parser::ErrorCode errorCode)
{
    for (size_t i = 0; i < result->size(); ++i)
    {
        media_download_list_ptr downloadList = result->at(i);
        if (!downloadList || !downloadList->size())
            continue;

        boost::uint64_t size 
#ifndef DOWNLOAD_MAX_VIDEO
            = (numeric_limits<uint64_t>::max)();
#else
            = (numeric_limits<uint64_t>::min)();
#endif

        int mp4index = -1;
        int index = -1;
        size_t currentFileId;

        {
            mutex::scoped_lock lock(outputInfoMutex);
            currentFileId = filesCount++;
            cout << "\n\nRESULT:";
            cout << "title:\t" << downloadList->title_utf8() << "\n";

            if (downloadList->thumbnail())
                cout << "jpeg size:\t" << downloadList->thumbnail()->size() << "\n";
            cout << "duration (sec): " << downloadList->duration() << "\n";

            for (size_t i = 0; i < downloadList->size(); ++i)
            {
                media_download_info mediaInfo = downloadList->media(i);
                cout << "\ntype:\t" << mediaInfo.content_type() << "\n";  
                cout << "size:\t" << mediaInfo.content_size() << " bytes\n";

                if ( mediaContentVideoMP4 == mediaInfo.content_type())
                    mp4index = (int) i;

#ifndef DOWNLOAD_MAX_VIDEO
                if (size > mediaInfo.content_size())
#else
                if (size < mediaInfo.content_size())
#endif
                {
                    size = mediaInfo.content_size();
                    index = (int)i;
                }

            }   
        }

        if (mp4index != -1 || index != -1)
        {
            const int dstIndex = (mp4index != -1) ? mp4index : index;
            string fileName = destDir + string("/") + lexical_cast<string>(currentFileId) + ".dmp";

            downloaders.push_back ( 
                shared_ptr<Downloader> (
                new Downloader( 
                downloadList->media(dstIndex).get_media_url_handle(),
                downloadList->media(dstIndex).get_media_info_handle(),
                fileName, lexical_cast<string>(currentFileId),
                outputInfoMutex)
                )            
                );
        }
    }
}

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        cerr << "usage: download_youtube [DESTDIR] [URL] [URL] ...";
        return 1;
    }
    cout << "count " << (argc - 2) << "\n";

    destDir = argv[1];

    for (int i = 2; i < argc; ++i)
    {
        const std::string url  = argv[i];
        media_site_type_t siteType = media_site_utils::validate_url(url);

        if ( mediaSiteUnknown == siteType)
        {
            cerr << "unknown site\n";
            continue;
        }   

        cout << "site type: " << siteType << "\n";

        url_parser::parse_url_async(url, url_parser::parseNormal, &OpenCallback);
    }

    char ch;
    std::cin >> ch;
    return 0;
}

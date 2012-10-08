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

#include <openmedia/DTHttpDownloader.h>
#include <iostream>
#include <boost/thread.hpp>
#include <boost/asio/error.hpp>
#include "../common/utils.h"

#ifdef _MSC_VER
#   pragma comment(lib, "dtdownloadsdk-static.lib")
#   pragma comment(lib, "dtcommonsdk-static.lib")

#   pragma comment(lib, "liburdl.lib")
#endif

using namespace std;
using namespace boost;
using namespace openmedia::downloader;

mutex guard;

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

class Downloader
{
public:
    Downloader( const string & Url,
                const string & Cookies,
                const string & OutputFileName,
                const string & DownloadName,
                mutex & Guard) : mutex_(Guard),
            name_(DownloadName)
    {
        init(Url, Cookies, OutputFileName);
    }

    void cancel()
    {
        http_downloader_->cancel();      
    }

    void pause()
    {
        http_downloader_->pause();        
    }

    void resume()
    {
        http_downloader_->resume();        
    }

private:
    void init(const string & Url,
              const string & Cookies,
              const string & OutputFileName)
    {
        http_downloader_.reset(
                new http_downloader( 
                        Url,
                        Cookies,
                        OutputFileName,
                        bind(&Downloader::StateNotify, this, _1),
                        bind(&Downloader::ProgressNotify, this, _1) )
                        );
        
    }

private:
    string name_;
    scoped_ptr< http_downloader > http_downloader_;
    
    void ProgressNotify(uint64_t Bytes)
    {
        mutex::scoped_lock lock(mutex_);
        cout << "progress: " << name_ << " " << Bytes << "\n";
    }

    void StateNotify(http_downloader::download_state_t State)
    {
        mutex::scoped_lock lock(mutex_);
        cout << "state: " << name_ << " " << State << "\n";
    }

    mutable mutex & mutex_;
};

int main(int argc, char* argv[])
{
    string url = "http://upload.wikimedia.org/wikipedia/en/8/80/Wikipedia-logo-v2.svg";
    string dest = "c:/temp/image.svg";

    if (argc == 3)
    {
        url = argv[1];
        dest = argv[2];
    }

    Downloader d1(url, "", dest, "downloader_1", guard);

    char ch;
    cin >> ch;

	return 0;
}


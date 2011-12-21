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

boost::mutex guard;

template <class T>
T & operator << (T & _Ostream, openmedia::downloader::http_downloader::download_state_t _State)
{
    if (_State == openmedia::downloader::http_downloader::stateStart)
        _Ostream << "[start]";
    else if (_State == openmedia::downloader::http_downloader::stateCancel)
        _Ostream << "[cancel]";
    else if (_State == openmedia::downloader::http_downloader::stateDownload)
        _Ostream << "[download]";
    else if (_State == openmedia::downloader::http_downloader::stateError)
        _Ostream << "[error]";
    else if (_State == openmedia::downloader::http_downloader::stateFinish)
        _Ostream << "[finish]";
    else if (_State == openmedia::downloader::http_downloader::statePause)
        _Ostream << "[pause]";
    else if (_State == openmedia::downloader::http_downloader::stateResume)
        _Ostream << "[resume]";
    else  _Ostream << "[unknown]";

    return _Ostream;
}

class Downloader
{
public:
    Downloader(const std::string & Url, const std::string & Cookies, const std::string OutputFileName, const std::string & DownloadName, boost::mutex & Guard) :
      mutex_(Guard),
      name_(DownloadName),
      http_downloader_(
          new openmedia::downloader::http_downloader( Url, Cookies, OutputFileName, boost::bind(&Downloader::StateNotify, this, _1), boost::bind(&Downloader::ProgressNotify, this, _1) )
          )
      {}

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
    std::string name_;
    boost::scoped_ptr< openmedia::downloader::http_downloader > http_downloader_;
    
    void ProgressNotify(boost::uint64_t Bytes)
    {
        boost::mutex::scoped_lock lock(mutex_);
        std::cout << "progress: " << name_ << " " << Bytes << "\n";
    }

    void StateNotify(openmedia::downloader::http_downloader::download_state_t State)
    {
        boost::mutex::scoped_lock lock(mutex_);
        std::cout << "\n ********** State: " << name_ << " " << State << "\n";
    }

    mutable boost::mutex & mutex_;

};

#if defined(_MSC_VER) && defined(_UNICODE)
int wmain(int argc, wchar_t * argv[])
#else
int main(int argc, char* argv[])
#endif
{
    std::string url3 = "http://img20.megavideo.com/633dfe45f4196be3e1a015f0f2649e43.jpg";
    Downloader d1( url3, "", "g:/test/test01.jpg", "D1", guard);

	return 0;
}

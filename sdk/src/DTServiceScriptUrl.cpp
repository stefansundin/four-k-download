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


#include <openmedia/DTHeaders.h>

#define BOOST_THREAD_USE_LIB
#include "DTServiceScript.h"

#include <string>
#include <sstream>

#include <openmedia/DTMediaDownloader.h>
#include <openmedia/DTHttpDownloader.h>
#include <boost/scoped_ptr.hpp>
#include <boost/thread.hpp>

namespace openmedia { namespace downloader {

namespace {
class Downloader
{
public:
    Downloader(const std::string & Url, const std::string & Cookies, std::ostream & OutputStream) :
      onExit_(false),
      error_(false),
      http_downloader_(
          new openmedia::downloader::http_downloader( Url, Cookies, OutputStream, 
          boost::bind(&Downloader::StateNotify, this, _1), 
          boost::bind(&Downloader::ProgressNotify, this, _1) )
          )
      {
          while (!onExit_)
          {
              boost::this_thread::sleep( boost::posix_time::milliseconds(100) ); 
          }
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

      bool error()
      {
          return error_;      
      }

private:
    bool onExit_;
    bool error_;
    boost::scoped_ptr< openmedia::downloader::http_downloader > http_downloader_;
    
    void ProgressNotify(boost::uint64_t/* Bytes*/)
    {
    }

    void StateNotify(http_downloader::download_state_t State)
    {
        switch(State)
        {
        case http_downloader::stateCancel:
        case http_downloader::stateNull:
        case http_downloader::stateError:
        default:
            error_= true;
        case http_downloader::stateFinish:
            onExit_ = true;
            break;
        case http_downloader::stateResume:
        case http_downloader::stateDownload:
        case http_downloader::stateStart:
        case http_downloader::statePause:
            break;
        }
    }
};

}

void set_service_script_url_impl(media_site_type_t MediaSite, unsigned int Id, const std::string & Url)
{
    std::stringstream strm;
    Downloader d(Url, "", strm);
    if (!d.error())
    {
        set_service_script(MediaSite, Id, strm.str());
    }
}

void set_service_script_url(media_site_type_t MediaSite, unsigned int Id, const std::string & Url)
{
    set_service_script_url_impl(MediaSite, Id, Url);
}

} }

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
#include <boost/lexical_cast.hpp>

#include "../common/utils.h"

#ifdef _MSC_VER
#   pragma comment(lib, "dtdownloadsdk-static.lib")
#   pragma comment(lib, "dtcommonsdk-static.lib")

#   pragma comment(lib, "liburdl.lib")
#endif

std::string destDir;

template <class T>
T & operator << (T & _Ostream, openmedia::downloader::media_downloader::download_state_t _State)
{
    if (_State == openmedia::downloader::media_downloader::stateStart)
        _Ostream << "[start]";
    else if (_State == openmedia::downloader::media_downloader::stateCancel)
        _Ostream << "[cancel]";
    else if (_State == openmedia::downloader::media_downloader::stateDownload)
        _Ostream << "[download]";
    else if (_State == openmedia::downloader::media_downloader::stateError)
        _Ostream << "[error]";
    else if (_State == openmedia::downloader::media_downloader::stateFinish)
        _Ostream << "[finish]";
    else if (_State == openmedia::downloader::media_downloader::statePause)
        _Ostream << "[pause]";
    else if (_State == openmedia::downloader::media_downloader::stateResume)
        _Ostream << "[resume]";
    else  _Ostream << "[unknown]";

    return _Ostream;
}

boost::mutex outputInfoMutex;

class Downloader
{
public:
    Downloader(::openmedia::downloader::media_url_handle_ptr DownloadHandle, 
        ::openmedia::downloader::media_info_handle_ptr DownloadMediaInfo,
        const std::string OutputFileName, const std::string & DownloadName, boost::mutex & Guard) :
      mutex_(Guard),
      name_(DownloadName),
      media_downloader_(
          new openmedia::downloader::media_downloader( DownloadHandle, DownloadMediaInfo, OutputFileName, 
          boost::bind(&Downloader::StateNotify, this, _1), 
          boost::bind(&Downloader::ProgressNotify, this, _1) )
          )
      {}

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
    std::string name_;
    boost::scoped_ptr< openmedia::downloader::media_downloader > media_downloader_;
    
    void ProgressNotify(boost::uint64_t Bytes)
    {
        boost::mutex::scoped_lock lock(mutex_);
        std::cout << "progress: " << name_ << " " << Bytes << "\n";
    }

    void StateNotify(openmedia::downloader::media_downloader::download_state_t State)
    {
        boost::mutex::scoped_lock lock(mutex_);
        std::cout << "\n ********** State: " << name_ << " " << State << "\n";
    }

    mutable boost::mutex & mutex_;

};

size_t filesCount = 0;

std::vector< boost::shared_ptr< Downloader > > downloaders;

#define DOWNLOAD_MAX_VIDEO

void OpenCallback(openmedia::downloader::url_parser_result_ptr result, openmedia::downloader::url_parser::ErrorCode errorCode)
{
    for (size_t i = 0; i < result->size(); ++i)
    {
        openmedia::downloader::media_download_list_ptr downloadList = result->at(i);
        if (!downloadList || !downloadList->size())
            continue;

        boost::uint64_t size 
#ifndef DOWNLOAD_MAX_VIDEO
            = (std::numeric_limits<boost::uint64_t>::max)();
#else
            = (std::numeric_limits<boost::uint64_t>::min)();
#endif

        int mp4index = -1;
        int index = -1;
        size_t currentFileId;

        {
            boost::mutex::scoped_lock lock(outputInfoMutex);
            currentFileId = filesCount++;
            std::cout << "\n\nRESULT:";

#ifdef _MSC_VER
            std::wcout << "title:\t" << downloadList->title() << "\n";
#endif
            std::cout << "title:\t" << downloadList->title_utf8() << "\n";

            if (downloadList->thumbnail())
                std::cout << "jpeg size:\t" << downloadList->thumbnail()->size() << "\n";
            std::cout << "duration (sec): " << downloadList->duration() << "\n";

            for (size_t i = 0; i < downloadList->size(); ++i)
            {
                openmedia::downloader::media_download_info mediaInfo = downloadList->media(i);
                std::cout << "\ntype:\t" << mediaInfo.content_type() << "\n";  
                std::cout << "size:\t" << mediaInfo.content_size() << " bytes\n";

                if ( ::openmedia::downloader::mediaContentVideoMP4 == mediaInfo.content_type())
                {
                    mp4index = (int) i;
                }

#ifndef DOWNLOAD_MAX_VIDEO
                if (size > mediaInfo.content_size())
#else
                if (size < mediaInfo.content_size())
#endif
                {
                    size = mediaInfo.content_size();
                    index = (int)i;
                }

                std::cout << "resolution:\t" << mediaInfo.width() << "x" << mediaInfo.height() << "\n";
            }   

        }

        if (mp4index != -1 || index != -1)
        {
            int dstIndex = (mp4index != -1) ? mp4index : index;

            ::std::string fileName = destDir + ::std::string("/") + boost::lexical_cast< std::string >(currentFileId) + ".dmp";

            downloaders.push_back
                (
                boost::shared_ptr< Downloader >
                (
                new Downloader( downloadList->media(dstIndex).get_media_url_handle(), downloadList->media(dstIndex).get_media_info_handle(), fileName, boost::lexical_cast< std::string >(currentFileId), outputInfoMutex)
                )            
                );
        }
    }

}

#if defined(_MSC_VER) && defined(_UNICODE)
int wmain(int argc, wchar_t * argv[])
#else
int main(int argc, char* argv[])
#endif
{
    if (argc < 3)
    {
        std::cerr << "usage: download_youtube.exe [DESTDIR] [URL] [URL] ...";
        return 1;
    }

    ::openmedia::downloader::service_script_info_list infoList;
    infoList.push_back( ::openmedia::downloader::service_script_info
        ( 
        ::openmedia::downloader::mediaSiteMegavideo, 0, "http://www.4kdownload.com/files/script/services/megavideo-2.txt"
        ) );

    set_service_script_list(infoList);

    std::cout << "count " << (argc - 2) << "\n";

    destDir
#if defined(_MSC_VER) && defined(_UNICODE)
        = utf16_to_ansi(argv[1]);
#else
        = argv[1];
#endif

    for (int i = 2; i < argc; ++i)
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

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


/// \file   get_download_info.cpp

#include <openmedia/DTMediaDownloader.h>
#include <openmedia/DTMediaSite.h>
#include <openmedia/DTHttpDownloader.h>
#include <openmedia/../../src/DTMediaDownloaderDetails.h>
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

template <class T>
T & operator << (T & Strm, media_video_type_t _Type)
{
    switch (_Type)
    {
    case mediaVideoNull:    Strm << "NULL"; break;
    default: 
    case mediaVideoUnknown: Strm << "Unknown"; break;
    case mediaVideoH264:    Strm << "H264"; break;
    case mediaVideoH263:    Strm << "H263"; break;
    case mediaVideoMPEG4:   Strm << "MPEG4"; break;
    case mediaVideoVP8:     Strm << "VP8"; break;
    }
    return Strm;
}

template <class T>
T & operator << (T & Strm, media_audio_type_t _Type)
{
    switch (_Type)
    {
    case mediaAudioNull: Strm << "NULL"; break;
    default: case mediaAudioUnknown: Strm << "Unknown"; break;
    case mediaAudioAAC: Strm << "AAC"; break;
    case mediaAudioMP3: Strm << "MP3"; break;
    case mediaAudioAMR: Strm << "AMR"; break;
    case mediaAudioVorbis: Strm << "Vorbis"; break;
    }
    return Strm;
}

template <class T>
T & operator << (T & Strm, media_content_type_t _Type)
{
    switch (_Type)
    {
    case mediaContentNull:      Strm << "NULL"; break;
    default: 
    case mediaContentUnknown:   Strm << "Unknown"; break;
    case mediaContentVideoFlv:  Strm << "FLV";  break;
    case mediaContentVideoMP4:  Strm << "MP4";  break;
    case mediaContentVideo3GP:  Strm << "3GP";  break;
    case mediaContentVideoWebm: Strm << "Webm"; break;
    }
    return Strm;
}

template <class T>
T & operator << (T & Strm, media_quality_type_t _Quality)
{
    switch (_Quality)
    {
    case mediaQualityNull:      Strm << "NULL"; break;
    case mediaQualityUnknown:
    default:                    Strm << "Unknown"; break;
    case mediaQualityOriginal:  Strm << "Original"; break;
    case mediaQuality1080P:     Strm << "1080P"; break;
    case mediaQuality720P:      Strm << "720P"; break;
    case mediaQuality480P:      Strm << "480P"; break;
    case mediaQuality360P:      Strm << "360P"; break;
    case mediaQuality240P:      Strm << "240P"; break;
    }
    return Strm;
}

void ProgressNotify(unsigned int Id, boost::uint64_t Bytes)
{
    cout << "progress: " << Id << " " << Bytes << "\r";
}

void StateNotify(unsigned int Id, http_downloader::download_state_t State)
{
    cout << "State: " << Id << " " << State << "\n";
}

class OnInitializedList
{
public:
    OnInitializedList(media_download_list_ptr _List) : list_(_List)
    {
    }

    void operator()(media_download_list::InitializedResult Result)
    {
        if (Result != media_download_list::initFailed)
        {
             cout << "title:\t" << list_->title_utf8() << "\n";
            if (list_->thumbnail())
                cout << "jpeg size:\t" << list_->thumbnail()->size() << "\n";

            const size_t subsCount = list_->subtitles_count();

            cout << "duration (sec): " << list_->duration() << "\n";
            
            if (list_->size())
            {
                std::cout << list_->media(0).get_media_url_handle()->url() << "\n";
            }
           
        }
    }
    

private:
    media_download_list_ptr list_;

};

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        cerr << "usage: get_download_info.exe [URL]";
        return 1;
    }

    string url = argv[1];
    media_site_type_t siteType = media_site_utils::validate_url(url);

    if ( mediaSiteUnknown == siteType)
    {
        cerr << "unknown site\n";
        return 1;
    }   

    cout << "Site type: " << siteType << "\n";
    
    url_parser_result_ptr result 
        = url_parser::parse_url(url, 
        media_site_utils::is_contain_playlist(url) ? url_parser::parsePlaylist : url_parser::parseNormal
        );

    if (!result)
    {
        cerr << "ERROR" << "\n";
    }
    else
    {
        for (size_t i = 0; i < result->size(); ++i)
        {
            if (!result->at(i)->initialized())
                result->at(i)->initialize( OnInitializedList(result->at(i)) );
            else
            {

                media_download_list_ptr downloadList = result->at(i);
                if (!downloadList || !downloadList->size())
                    continue;
                cout << "title:\t" << downloadList->title_utf8() << "\n";

                if (downloadList->thumbnail())
                    cout << "jpeg size:\t" << downloadList->thumbnail()->size() << "\n";

                cout << "duration (sec): " << downloadList->duration() << "\n";
                cout << "subtitles count : " << downloadList->subtitles_count() << "\n";

                for (size_t i = 0; i < downloadList->subtitles_count(); ++i)
                {
                    if (argc >= 3)
                    {
                        const std::string fileName = std::string(argv[2]) + "_" + 
                            downloadList->subtitle_id_at(i) + ".srt";
                        downloadList->subtitle_save(i, fileName);
                    }

                    cout << "ID: " << downloadList->subtitle_id_at(i) << ": " << downloadList->subtitle_lang_at(i) << "\n";                
                    
                }

                std::cout << "\n";

                for (size_t i = 0; i < downloadList->size(); ++i)
                {
                    media_download_info mediaInfo = downloadList->media(i);
                    cout << "\ntype:\t" << mediaInfo.content_type() << "\n";  
                    cout << "size:\t" << mediaInfo.content_size() << " bytes\n";
                    cout << "quality:\t" << mediaInfo.quality() << "\n";
                    cout << "video:\t" << mediaInfo.video_codec() << "\n";
                    cout << "audio:\t" << mediaInfo.audio_codec() << "\n";
                    cout << "URL:\t" << mediaInfo.get_media_url_handle()->url() << "\n";
                }
            }
        }
    }

    char ch;
    cin >> ch;
	return 0;
}


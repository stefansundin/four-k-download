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


#include <openmedia/DTMediaDownloader.h>
#include <openmedia/DTMediaSite.h>
#include <openmedia/DTHttpDownloader.h>
#include <openmedia/../../src/DTMediaDownloaderDetails.h>
#include <iostream>

#include "../common/utils.h"

#ifdef _MSC_VER
#   pragma comment(lib, "dtdownloadsdk-static.lib")
#   pragma comment(lib, "dtcommonsdk-static.lib")

#   pragma comment(lib, "liburdl.lib")
#endif

template <class T>
T & operator << (T & _Ostream, openmedia::downloader::media_video_type_t _Type)
{
    using namespace  openmedia::downloader;

    switch (_Type)
    {
    case mediaVideoNull: _Ostream << "NULL"; break;
    default: 
    case mediaVideoUnknown: _Ostream << "Unknown"; break;
    case mediaVideoH264: _Ostream << "H264"; break;
    case mediaVideoH263: _Ostream << "H263"; break;
    case mediaVideoMPEG4: _Ostream << "MPEG4"; break;
    case mediaVideoVP8: _Ostream << "VP8"; break;
    }
    return _Ostream;
}

template <class T>
T & operator << (T & _Ostream, openmedia::downloader::media_audio_type_t _Type)
{
    using namespace  openmedia::downloader;

    switch (_Type)
    {
    case mediaAudioNull: _Ostream << "NULL"; break;
    default:
    case mediaAudioUnknown: _Ostream << "Unknown"; break;
    case mediaAudioAAC: _Ostream << "AAC"; break;
    case mediaAudioMP3: _Ostream << "MP3"; break;
    case mediaAudioAMR: _Ostream << "AMR"; break;
    case mediaAudioVorbis: _Ostream << "Vorbis"; break;
    }
    return _Ostream;
}

template <class T>
T & operator << (T & _Ostream, openmedia::downloader::media_content_type_t _Type)
{
    using namespace  openmedia::downloader;

    switch (_Type)
    {
    case mediaContentNull: _Ostream << "NULL"; break;
    default:
    case mediaContentUnknown: _Ostream << "Unknown"; break;
    case mediaContentVideoFlv: _Ostream << "FLV"; break;
    case mediaContentVideoMP4: _Ostream << "MP4"; break;
    case mediaContentVideo3GP: _Ostream << "3GP"; break;
    case mediaContentVideoWebm: _Ostream << "Webm"; break;
    }
    return _Ostream;
}

template <class T>
T & operator << (T & _Ostream, openmedia::downloader::media_quality_type_t _Quality)
{
    using namespace  openmedia::downloader;
    switch (_Quality)
    {
    case mediaQualityNull: _Ostream << "NULL"; break;
    case mediaQualityUnknown:
    default: _Ostream << "Unknown"; break;
    case mediaQualityOriginal: _Ostream << "Original"; break;
    case mediaQuality1080P: _Ostream << "1080P"; break;
    case mediaQuality720P: _Ostream << "720P"; break;
    case mediaQuality480P: _Ostream << "480P"; break;
    case mediaQuality360P: _Ostream << "360P"; break;
    case mediaQuality240P:  _Ostream << "240P"; break;
    }
    return _Ostream;
}

void ProgressNotify(unsigned int Id, boost::uint64_t Bytes)
{
    std::cout << "progress: " << Id << " " << Bytes << "\r";
}

void StateNotify(unsigned int Id, openmedia::downloader::http_downloader::download_state_t State)
{
    std::cout << "State: " << Id << " " << State << "\n";
}

class OnInitializedList
{
public:
    OnInitializedList(openmedia::downloader::media_download_list_ptr _List) : list_(_List)
    {
    }

    void operator()(openmedia::downloader::media_download_list::InitializedResult Result)
    {
        if (Result != openmedia::downloader::media_download_list::initFailed)
        {
             std::cout << "title:\t" << list_->title_utf8() << "\n";
            if (list_->thumbnail())
                std::cout << "jpeg size:\t" << list_->thumbnail()->size() << "\n";

            std::cout << "duration (sec): " << list_->duration() << "\n";
            
        }
    }
    

private:
    openmedia::downloader::media_download_list_ptr list_;

};

using namespace openmedia;

#if defined(_MSC_VER) && defined(_UNICODE)
int wmain(int argc, wchar_t * argv[])
#else
int main(int argc, char* argv[])
#endif
{
    if (argc < 2)
    {
        std::cerr << "usage: get_download_info.exe [URL]";
        return 1;
    }

    std::string url 
#if defined(_MSC_VER) && defined(_UNICODE)
        = utf16_to_ansi(argv[1]);
#else
        = argv[1];
#endif

    downloader::media_site_type_t siteType = downloader::media_site_utils::validate_url(url);

    if ( downloader::mediaSiteUnknown == siteType)
    {
        std::cerr << "unknown site\n";
        return 1;
    }   

    std::cout << "Site type: " << siteType << "\n";

    downloader::url_parser_result_ptr result 
        = downloader::url_parser::parse_url(url, 
        downloader::media_site_utils::is_contain_playlist(url) ? downloader::url_parser::parsePlaylist : downloader::url_parser::parseNormal
        );

    if (!result)
    {
        std::cerr << "ERROR" << "\n";
    }
    else
    {
        for (size_t i = 0; i < result->size(); ++i)
        {
            if (!result->at(i)->initialized())
                result->at(i)->initialize( OnInitializedList(result->at(i)) );
            else
            {

                downloader::media_download_list_ptr downloadList = result->at(i);
                if (!downloadList || !downloadList->size())
                    continue;

#if defined(_MSC_VER) && defined(_UNICODE)
                std::wcout << L"title:\t" << downloadList->title() << L"\n";
#endif

                std::cout << "title:\t" << downloadList->title_utf8() << "\n";

                if (downloadList->thumbnail())
                    std::cout << "jpeg size:\t" << downloadList->thumbnail()->size() << "\n";

                std::cout << "duration (sec): " << downloadList->duration() << "\n";

                for (size_t i = 0; i < downloadList->size(); ++i)
                {
                    downloader::media_download_info mediaInfo = downloadList->media(i);
                    std::cout << "\ntype:\t" << mediaInfo.content_type() << "\n";  
                    std::cout << "size:\t" << mediaInfo.content_size() << " bytes\n";
                    std::cout << "quality:\t" << mediaInfo.quality() << "\n";
                    std::cout << "video:\t" << mediaInfo.video_codec() << "\n";
                    std::cout << "audio:\t" << mediaInfo.audio_codec() << "\n";

                    std::cout << "resolution:\t" << mediaInfo.width() << "x" << mediaInfo.height() << "\n";
                }
            }
        }
    }

	return 0;
}


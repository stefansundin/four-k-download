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


/// \file   DTMediaDownloader.h
#ifndef _DT_MEDIADOWNLOADER_H_
#define _DT_MEDIADOWNLOADER_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <string>
#include <exception>
#include <vector>

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

#include <openmedia/DTMediaSite.h>

namespace openmedia { namespace downloader {

struct service_script_info
{
    service_script_info(): mediaSite(mediaSiteNull), id(0), url("")
    {}

    service_script_info(media_site_type_t MediaSite, unsigned int Id, const std::string & Url): 
    mediaSite(MediaSite), id(Id), url(Url)
    {}

    media_site_type_t   mediaSite;
    unsigned int        id;
    std::string         url;
};

typedef ::std::vector<service_script_info> service_script_info_list;
void set_service_script_list(service_script_info_list & _List);

enum media_content_type_t
{
    mediaContentNull = 0,
    mediaContentUnknown,
    mediaContentVideoFlv,
    mediaContentVideoMP4,
    mediaContentVideo3GP,
    mediaContentVideoWebm

};

enum media_video_type_t
{
    mediaVideoNull = 0,
    mediaVideoUnknown,
    mediaVideoH264,
    mediaVideoH263,
    mediaVideoMPEG4,
    mediaVideoVP8
};

enum media_audio_type_t
{
    mediaAudioNull = 0,
    mediaAudioUnknown,
    mediaAudioAAC,
    mediaAudioMP3,
    mediaAudioAMR,
    mediaAudioVorbis    
};

enum media_quality_type_t
{
    mediaQualityNull = 0,
    mediaQualityUnknown,
    mediaQualityOriginal,
    mediaQuality1080P,
    mediaQuality720P_1,
    mediaQuality720P,
    mediaQuality480P,
    mediaQuality360P_1,
    mediaQuality360P,
    mediaQuality240P_1,
    mediaQuality240P,
    mediaQualityQCIF    
};

int get_media_similarity(media_quality_type_t Quality1, 
                         media_content_type_t Format1,
                         media_quality_type_t Quality2,
                         media_content_type_t Format2);

media_quality_type_t select_media_quality(int height);

class media_download_info;
typedef boost::shared_ptr<media_download_info> media_download_info_ptr;

class media_url_handle;
typedef boost::shared_ptr<media_url_handle> media_url_handle_ptr;

class media_info_handle;
typedef boost::shared_ptr<media_info_handle> media_info_handle_ptr;

std::string get_media_url(media_url_handle * UrlHandle);

/// \class  media_download_info
class media_download_info
{
public:
    media_download_info()
    {}

    media_download_info(
        media_url_handle_ptr MediaUrlHandle,
        media_info_handle_ptr MediaInfoHandle
        )         : 
        media_url_handle_(MediaUrlHandle),
        media_info_handle_(MediaInfoHandle)
        {
        };
        
    media_url_handle_ptr    get_media_url_handle() const { return media_url_handle_; }
    media_info_handle_ptr   get_media_info_handle() const { return media_info_handle_; }
   
    media_content_type_t    content_type() const; 
    boost::uint64_t         content_size() const;

    int                     width() const;
    int                     height() const;

    media_quality_type_t    quality() const;
    media_video_type_t      video_codec() const;
    media_audio_type_t      audio_codec() const;


private:
    media_url_handle_ptr media_url_handle_;
    media_info_handle_ptr media_info_handle_;
};

bool compare_media_download_info(const media_download_info & i1, const media_download_info & i2);

class url_parser_result;
typedef boost::shared_ptr<url_parser_result> url_parser_result_ptr;

class media_download_list;
typedef boost::shared_ptr<media_download_list> media_download_list_ptr;

/// \class  url_parser
class url_parser
{
public:
    enum ErrorCode
    {
        errorNone = 0,
        errorUnknown = 1,
        errorNoElements = 2
    };

    class exception : public virtual ::std::exception
    {
    public:
        exception() : ::std::exception(), error_code_(errorUnknown)
        {}

        exception(ErrorCode error_code) : ::std::exception(), error_code_(error_code)
        {}

        exception(ErrorCode error_code, const char *const& _What) : ::std::exception(), error_code_(error_code)
        {
            what_ = _What;
        }

        virtual const char * what() const throw()
        {
            return what_;
        }

    private:
        const char * what_;
        ErrorCode error_code_;
    };

    typedef boost::function< void (url_parser_result_ptr, ErrorCode) > ParseUrlResult;

    enum ParseMode {
        parseNormal = 0,
        parsePlaylist
    };

    static url_parser_result_ptr parse_url(const std::string & Url, ParseMode mode);
    static void parse_url_async(const std::string & Url, ParseMode mode, ParseUrlResult ParseUrlResultCallback);
};

/// \class  media_download_list
class media_download_list : boost::noncopyable
{
public:
    class Impl;

    typedef std::vector< boost::uint8_t > BytesArray;
    typedef boost::shared_ptr<BytesArray> BytesArrayPtr;

public:
    std::wstring            title() const;
    std::string             title_utf8() const;
    BytesArrayPtr           thumbnail() const;
    boost::uint32_t         duration() const;
    std::string             url() const;
   
    size_t                      size() const;
    const media_download_info & media(size_t Index) const;
    int                         find(media_quality_type_t Quality, media_content_type_t Content) const;
    bool                        initialized() const;

    size_t                  subtitles_count();
    std::string             subtitle_id_at(size_t index);
    std::string             subtitle_lang_at(size_t index);
    void                    subtitle_save(size_t index, const std::string & fileNameUtf8);

public:
    enum InitializedResult
    {
        initAlready,
        initSuccess,
        initFailed
    };

    typedef boost::function< void (InitializedResult) > OnInitialized;
    void initialize(OnInitialized onInitialized);

public:
    media_download_list(boost::shared_ptr<Impl> impl, bool Intitalized = true);
    void reset(boost::shared_ptr<Impl> impl);
    boost::shared_ptr<Impl> release();

private:
    boost::shared_ptr<Impl> impl_;
    void on_initialize_(OnInitialized onInitialized, url_parser_result_ptr result, url_parser::ErrorCode errorCode);
    bool initialized_;
};

/// \struct media_desc_type
struct media_desc_type
{
    media_quality_type_t quality;
    media_content_type_t format;
    media_video_type_t video;
    media_audio_type_t audio;
};

inline bool operator < (const media_desc_type & t1, const media_desc_type & t2)
{
    return t1.quality < t2.quality;
}

/// \class  url_parser_result
class url_parser_result
{
public:
    size_t size() const;
    media_download_list_ptr at(size_t Index) const;
    std::vector< media_desc_type > qualities() const;

public:
    class Impl;
    url_parser_result(Impl * _impl);
    Impl * impl_;

};

/// \class  media_downloader
class media_downloader : boost::noncopyable
{
public:
    enum command_result_t
    {
        resultFail = 0,
        resultOk = 1,
        resultNotFound = 2
    };
    
    enum download_state_t 
    {
        stateNull = 0,
        statePending,
        stateStart,
        stateCancel,
        stateFinish,
        stateDownload,
        statePause,
        stateResume,
        stateError,
        stateCount
    };

    typedef boost::function< void (boost::uint64_t) >   DownloadProgressNotify;
    typedef boost::function< void (download_state_t) >    DownloadStateNotify;

public:
    media_downloader(media_url_handle_ptr MediaUrl, media_info_handle_ptr MediaInfo, const std::string & OutputFilename, DownloadStateNotify StateNotify, DownloadProgressNotify ProgressNotify);
    media_downloader(media_url_handle_ptr MediaUrl, media_info_handle_ptr MediaInfo, const std::wstring & OutputFilename, DownloadStateNotify StateNotify, DownloadProgressNotify ProgressNotify);

    ~media_downloader();
    

    command_result_t pause();
    command_result_t resume();
    command_result_t cancel();

    static void shutdown();

public:
    class Impl;
private:
    Impl * impl_;
};

} } // namespace openmedia::downloader

#endif // #define _DT_MEDIADOWNLOADER_H_

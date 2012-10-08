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



// precompiled header begin
#include "DTHeadersVideoDownload.h"
// precompiled header end

#include <string>
#include <vector>
#include <iostream>
#include <set>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/static_assert.hpp>

#include <openmedia/DTMediaDownloader.h>
#include <openmedia/DTMediaSite.h>
#include <openmedia/DTHttpDownloader.h>
#include <openmedia/DTAssert.h>
#include <openmedia/DTError.h>

#include "DTMediaDownloaderDetails.h"
#include "DTDownloaderYoutube.h"
#include "videodownload/DTDownloaderRaiTv.h"
#include "DTDownloaderVimeo.h"
#include "DTDownloaderDailymotion.h"
#include "DTDownloaderFacebook.h"
#include "DTDownloaderMetacafe.h"

#include "DTMediaDownloaderImpl.h"

#include "DTLog.h"
#include "videodownload/DTSubtitle.h"

namespace openmedia { namespace downloader {

BOOST_STATIC_ASSERT( media_downloader::stateCount == http_downloader::stateCount);

media_content_type_t parse_media_content_type(const std::string & MediaContentStr)
{
    if (MediaContentStr == "video/flv")
        return mediaContentVideoFlv; 
    else if (MediaContentStr == "video/x-flv")
        return mediaContentVideoFlv;
    else if (MediaContentStr == "video/mp4")
        return mediaContentVideoMP4;
    else if (MediaContentStr == "video/3gpp")
        return mediaContentVideo3GP;
    else if (MediaContentStr == "video/webm")
        return mediaContentVideoWebm;
    else
        return mediaContentUnknown;
}

media_content_type_t media_download_info::content_type() const 
{
    const media_content_type_t t = parse_media_content_type(media_url_handle_->content_type()); 
    if (mediaContentUnknown == t)
        return media_info_handle_->format();
    else
        return t;
}

boost::uint64_t media_download_info::content_size() const 
{ 
    return media_url_handle_->content_size(); 
}

int media_download_info::width() const
{
    return media_info_handle_->width();
}

int media_download_info::height() const
{
    return media_info_handle_->height();
}

media_quality_type_t media_download_info::quality() const
{
    return media_info_handle_->quality();
}

media_video_type_t media_download_info::video_codec() const
{
    return media_info_handle_->video_codec();
}

media_audio_type_t media_download_info::audio_codec() const
{
    return media_info_handle_->audio_codec();
}

size_t media_download_list::size() const
{
    if (!impl_)
        return 0;
    else
        return impl_->size();
}

const media_download_info & media_download_list::media(size_t Index) const
{
    return impl_->media(Index);
}

int media_download_list::find(media_quality_type_t Quality, media_content_type_t Content) const
{
    return impl_->media(Quality, Content);
}

std::wstring media_download_list::title() const
{
    return impl_->title();
}

std::string media_download_list::title_utf8() const
{
    return impl_->title_utf8();
}

media_download_list::BytesArrayPtr media_download_list::thumbnail() const
{
    return impl_->thumbnail();
}

boost::uint32_t media_download_list::duration() const
{
    return impl_->duration();
}

std::string media_download_list::url() const
{
    return impl_->url();
}

media_download_list::media_download_list(boost::shared_ptr<Impl> impl, bool Intitalized) : impl_(impl), initialized_(Intitalized)
{
}

void media_download_list::reset(boost::shared_ptr<Impl> impl)
{
    impl_ = impl;
}

bool media_download_list::initialized() const
{
    return initialized_; 
}

boost::shared_ptr<media_download_list::Impl> media_download_list::release()
{
    boost::shared_ptr<media_download_list::Impl> imp = impl_;
    return impl_;    
}

void media_download_list::initialize(OnInitialized onInitialized)
{
    if (initialized())
    {
        if (onInitialized)
            onInitialized(initAlready);
        return;
    }
    else
    {
        url_parser::parse_url_async(impl_->url(), url_parser::parseNormal, boost::bind( &media_download_list::on_initialize_, this, onInitialized, _1, _2 ) );
    }
}

void media_download_list::on_initialize_(OnInitialized onInitialized, url_parser_result_ptr result, url_parser::ErrorCode errorCode)
{
    if (url_parser::errorNone == errorCode && result && result->size() > 0 && result->at(0))
    {
        boost::shared_ptr<media_download_list::Impl> newImpl = result->at(0)->release();
        this->reset(newImpl);
        if (onInitialized)
            onInitialized(initSuccess);
        return;
    }
    else
    {
        if (onInitialized)
            onInitialized(initFailed);
    }
}

size_t media_download_list::subtitles_count()
{
    if (impl_->subs())
        return impl_->subs()->count();
    else
        return 0;
}

std::string media_download_list::subtitle_id_at(size_t index)
{
    if (impl_->subs())
        return impl_->subs()->id_at(index);
    else
    {
        BOOST_THROW_EXCEPTION(errors::invalid_operation());
    }
}

std::string media_download_list::subtitle_lang_at(size_t index)
{
    if (impl_->subs())
        return impl_->subs()->lang_at(index);
    else
    {
        BOOST_THROW_EXCEPTION(errors::invalid_operation());
    }
}

void media_download_list::subtitle_save(size_t index, const std::string & fileNameUtf8)
{
    if (impl_->subs())
        return impl_->subs()->save(index, fileNameUtf8);
    else
    {
        BOOST_THROW_EXCEPTION(errors::invalid_operation());
    }
}

//

bool compare_media_download_info(const media_download_info & i1, const media_download_info & i2)
{
    return i1.quality() < i2.quality();
}

/////////////////////////////////////////////////////

class url_parser_result::Impl
{
public:
    Impl(media_download_list_ptr FirstElement)
    {
        if (FirstElement)
            result_.push_back(FirstElement);
    }

    Impl( const std::vector<media_download_list_ptr> & Elements)
    {
        result_ = Elements;
    }

    size_t size() const
    {
        return result_.size();
    }

    media_download_list_ptr at(size_t Index) const
    {
        return result_.at(Index);
    }

 private:
    std::vector<media_download_list_ptr> result_;
};

url_parser_result::url_parser_result(Impl * _impl) : impl_(_impl)
{}

size_t url_parser_result::size() const
{
    return impl_->size();
}

media_download_list_ptr url_parser_result::at(size_t Index) const
{
    return impl_->at(Index);
}

std::vector< media_desc_type > url_parser_result::qualities() const
{
    std::vector< media_desc_type > result;
    std::set< media_quality_type_t > q;
    for (size_t i = 0; i < size(); ++i)
    {
        if (media_download_list_ptr elm = at(i))
        {
            for (size_t j = 0; j < elm->size(); ++j )
            {
                if (q.count( elm->media(j).quality() ) == 0)
                {
                    q.insert(elm->media(j).quality());
                    media_desc_type desc = {elm->media(j).quality(),
                                            elm->media(j).content_type(),
                                            elm->media(j).video_codec(),
                                            elm->media(j).audio_codec()
                                           };
                    result.push_back(desc);
                }
            }

        }
    }
    std::sort(result.begin(), result.end());
    return result;
}

/////////////////////////////////////////////////////

url_parser_result_ptr url_parser::parse_url(const std::string & Url, url_parser::ParseMode mode)
{
    media_download_list_ptr result;
    try
    {
        switch (media_site_utils::validate_url(Url))
        {
        default:

        case mediaSiteUnknown:
            result = media_download_list_ptr();
            break;

        case mediaSiteYoutube:
        {
            if (url_parser::parseNormal == mode)
            {
                result =  downloader::youtube::parse_url(Url);
            }
            else if (url_parser::parsePlaylist == mode)
            {
                std::vector<media_download_list_ptr> resultList = downloader::youtube::parse_url_playlist(Url);
                return url_parser_result_ptr(
                            new url_parser_result(new url_parser_result::Impl(resultList) ));
            }
        }
            break;

        case mediaSiteRaiTv:
            result = downloader::raitv::parse_url(Url);
            break;

        case mediaSiteVimeo:
            result =  downloader::vimeo::parse_url(Url);
            break;

        case mediaSiteDailymotion:
            result =  downloader::dailymotion::parse_url(Url);
            break;

        case mediaSiteFacebook:
            result =  downloader::facebook::parse_url(Url);
            break;

        case mediaSiteMetacafe:
            result =  downloader::metacafe::parse_url(Url);
            break;

        }
    }
    catch(...)
    {
        return url_parser_result_ptr();
    }

    if (!result)
        return url_parser_result_ptr();

    return url_parser_result_ptr(
                new url_parser_result(new url_parser_result::Impl(result) ));
}

namespace 
{

void parse_url_impl(const std::string & Url, url_parser::ParseMode mode, url_parser::ParseUrlResult ParseUrlResultCallback)
{
    url_parser::ErrorCode resultError = url_parser::errorNone;
    url_parser_result_ptr result;

    try 
    {
        std::string url_to_parse = Url;
        if (!( boost::algorithm::contains(url_to_parse, "http://") || boost::algorithm::contains(url_to_parse, "https://") ))
            url_to_parse = "http://" + url_to_parse;

        boost::algorithm::replace_all(url_to_parse, "https://", "http://");
        result = url_parser::parse_url(url_to_parse, mode);
    }
    catch(url_parser::exception &)
    {
        resultError = url_parser::errorUnknown;         
    }
    catch(std::exception &)
    {
        resultError = url_parser::errorUnknown;         
    }
    catch(...)
    {
        resultError = url_parser::errorUnknown;             
    }

    if (!result || !result->size())
        resultError = url_parser::errorNoElements;

    if (ParseUrlResultCallback)
        ParseUrlResultCallback(result, resultError);
}
    
}

void url_parser::parse_url_async(const std::string & Url, url_parser::ParseMode mode, url_parser::ParseUrlResult ParseUrlResultCallback)
{
    boost::thread parseUrlThread = boost::thread(&parse_url_impl, Url, mode, ParseUrlResultCallback);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class media_downloader_Impl : public media_downloader::Impl
{
public:
    template <class String>
    media_downloader_Impl(  media_url_handle_ptr MediaUrl,
                            const String & OutputFilename,
                            media_downloader::DownloadStateNotify StateNotify,
                            media_downloader::DownloadProgressNotify ProgressNotify) 

        : state_notify_(StateNotify),
        progress_notify_(ProgressNotify),
        http_downloader_( new http_downloader(MediaUrl->url(), MediaUrl->cookies(), OutputFilename, 
        boost::bind(&media_downloader_Impl::StateNotify_, this, _1),
        boost::bind(&media_downloader_Impl::ProgressNotify_, this, _1) ) )
    {
    }

    template <class String>
    media_downloader_Impl(  media_url_handle_ptr MediaUrl,
                            const String & OutputFilename,
                            media_downloader::DownloadStateNotify StateNotify,
                            media_downloader::DownloadProgressNotify ProgressNotify,
                            size_t PartSizeBytes) 

        : state_notify_(StateNotify),
        progress_notify_(ProgressNotify),
        http_downloader_( new http_downloader(MediaUrl->url(), MediaUrl->cookies(), OutputFilename, 
        boost::bind(&media_downloader_Impl::StateNotify_, this, _1),
        boost::bind(&media_downloader_Impl::ProgressNotify_, this, _1), PartSizeBytes) )
    {
    }

    ~media_downloader_Impl()
    {
        DT_LOG(trace) << "destroy media_downloader::Impl\n";
    }

    media_downloader::command_result_t pause() 
    { 
        return static_cast<media_downloader::command_result_t>(http_downloader_->pause()); 
    }

    media_downloader::command_result_t resume() 
    {
        return static_cast<media_downloader::command_result_t>(http_downloader_->resume()); 
    }

    media_downloader::command_result_t cancel() 
    { 
        return static_cast<media_downloader::command_result_t>(http_downloader_->cancel()); 
    } 

private:
    media_downloader::DownloadStateNotify state_notify_;
    media_downloader::DownloadProgressNotify progress_notify_;
    
    boost::shared_ptr< http_downloader > http_downloader_;

    void StateNotify_( http_downloader::download_state_t State)
    {
        if (state_notify_)
            state_notify_( static_cast<media_downloader::download_state_t>( State ));    
    }

    void ProgressNotify_( boost::uint64_t SizeBytes)
    {
        if (progress_notify_)
            progress_notify_(SizeBytes);            
    }

};

namespace {

    const size_t YouTubePartSizeMagic = 35; 
    const size_t DailyMotionPartSizeMagic = 23; 

    template <class String>
    std::auto_ptr<media_downloader::Impl> create_media_downloader(
        media_url_handle_ptr MediaUrl, media_info_handle_ptr MediaInfo, const String & OutputFilename, media_downloader::DownloadStateNotify StateNotify, media_downloader::DownloadProgressNotify ProgressNotify
        )
    {
        //std::cout << "\n" << MediaUrl->url() << "\n";
        DT_LOG(trace) << "Content Size: " << MediaUrl->content_size() << "\n";
        if (MediaInfo->duration() > 0 && mediaSiteYoutube == media_site_utils::validate_url(MediaUrl->url()))
        {
            const size_t bytesPerSec = static_cast<size_t>(MediaUrl->content_size() / (1.0 * MediaInfo->duration() / 1000.0));
            const size_t partSize = YouTubePartSizeMagic * bytesPerSec;
            return std::auto_ptr<media_downloader::Impl>(
                new media_downloader_Impl(MediaUrl, OutputFilename, StateNotify, ProgressNotify, partSize)
                );           
        } else if (MediaInfo->duration() > 0 && mediaSiteDailymotion == media_site_utils::validate_url(MediaUrl->url()))
        {
            const size_t bytesPerSec = static_cast<size_t>(MediaUrl->content_size() / (1.0 * MediaInfo->duration() / 1000.0));
            const size_t partSize = DailyMotionPartSizeMagic * bytesPerSec;
            return std::auto_ptr<media_downloader::Impl>(
                new media_downloader_Impl(MediaUrl, OutputFilename, StateNotify, ProgressNotify, partSize)
                );           
        }
        else
        {
            return std::auto_ptr<media_downloader::Impl>(
                new media_downloader_Impl(MediaUrl, OutputFilename, StateNotify, ProgressNotify)
                );           
        }
    }
}

media_downloader::media_downloader(media_url_handle_ptr MediaUrl, media_info_handle_ptr MediaInfo, const std::string & OutputFilename, DownloadStateNotify StateNotify, DownloadProgressNotify ProgressNotify)
: impl_(NULL)
{
    boost::function< ::std::auto_ptr< media_downloader::Impl > () > createImplFun =
        boost::bind(&create_media_downloader<std::string>, MediaUrl, MediaInfo, OutputFilename, StateNotify, ProgressNotify); 

    impl_ = createImplFun().release();
}

media_downloader::media_downloader(media_url_handle_ptr MediaUrl, media_info_handle_ptr MediaInfo, const std::wstring & OutputFilename, DownloadStateNotify StateNotify, DownloadProgressNotify ProgressNotify)
: impl_(NULL)
{
    boost::function< ::std::auto_ptr< media_downloader::Impl > () > createImplFun =
        boost::bind(&create_media_downloader<std::wstring>, MediaUrl, MediaInfo, OutputFilename, StateNotify, ProgressNotify); 

    impl_ = createImplFun().release();

}


media_downloader::~media_downloader()
{
    DT_LOG(trace) << "destroy media downloader" << "\n";
    delete impl_;
}

media_downloader::command_result_t media_downloader::pause() 
{ 
    DT_LOG(debug) << "media_downloader::pause";
    DT_ASSERT(NULL != impl_);
    return impl_->pause(); 
}

media_downloader::command_result_t media_downloader::resume() 
{
    DT_LOG(debug) << "media_downloader::resume";
    DT_ASSERT(NULL != impl_);
    return impl_->resume(); 
}

media_downloader::command_result_t media_downloader::cancel() 
{ 
    DT_LOG(debug) << "media_downloader::cancel";
    DT_ASSERT(NULL != impl_);
    return impl_->cancel(); 
}

void media_downloader::shutdown()
{
    http_downloader::shutdown();
}

media_quality_type_t select_media_quality(int height)
{
    if (height <= 1)
        return mediaQualityUnknown;
    else if (height <= 240)
        return mediaQuality240P;
    else if (height <= 360)
        return mediaQuality360P;
    else if (height <= 480)
        return mediaQuality480P;
    else if (height <= 720)
        return mediaQuality720P;
    else if (height <= 1080)
        return mediaQuality1080P;
    else 
        return mediaQualityOriginal;
}

int get_media_similarity(media_quality_type_t Quality1, 
                         media_content_type_t Format1,
                         media_quality_type_t Quality2,
                         media_content_type_t Format2)
{
    if (Quality1 == Quality2 && Format1 == Format2)
        return 100;
    else if (Format1 == Format2)
        return 50 - (std::abs)(Quality1 - Quality2);
    else
        return 25 - (std::abs)(Quality1 - Quality2);
}

std::string get_media_url(media_url_handle * UrlHandle)
{
    return UrlHandle ? UrlHandle->url() : "";
}

} }

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


#ifndef _DT_MEDIADOWNLOADERDETAILS_H_
#define _DT_MEDIADOWNLOADERDETAILS_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <openmedia/DTMediaDownloader.h>
#include <vector>
#include <boost/foreach.hpp>
#include "DTDownloaderUtils.h"

namespace openmedia { namespace downloader {

class subtitles;
typedef boost::shared_ptr<subtitles> subtitles_ptr;

media_content_type_t parse_media_content_type(const std::string & MediaContentStr);

class media_info_handle
{
public:
    media_info_handle(  int Width, 
                        int Height, 
                        media_quality_type_t MediaQuality,
                        media_content_type_t MediaFormat,
                        media_video_type_t VideoCodec, 
                        media_audio_type_t AudioCodec, 
                        boost::int64_t Duration = 0) : 
        width_(Width), height_(Height), 
          media_quality_type_(MediaQuality),
          media_content_type_(MediaFormat),
          media_video_codec_type_(VideoCodec),
          media_audio_codec_type_(AudioCodec),
          duration_(Duration)
      {}

public:
    int width() const { return width_; }
    int height() const { return height_; }
    
    media_quality_type_t    quality() const { return media_quality_type_; }
    media_content_type_t    format() const { return media_content_type_; }
    media_video_type_t      video_codec() const { return media_video_codec_type_; }
    media_audio_type_t      audio_codec() const { return media_audio_codec_type_; }
    boost::int64_t          duration() const { return duration_; }

private:
    int width_;
    int height_;
    media_quality_type_t media_quality_type_;
    media_content_type_t media_content_type_;
    media_video_type_t media_video_codec_type_;
    media_audio_type_t media_audio_codec_type_;
    boost::int64_t duration_;

};

/// \class  media_url_handle
class media_url_handle
{
public:
    media_url_handle
        (
        const std::string & Url,
        const std::string & ContentType,
        boost::uint64_t ContentSize,
        const std::string & Cookies = "",
        const std::string & AdditionalHeaders = ""
        ) 
        : 
        url_(Url), 
        content_type_(ContentType), content_size_(ContentSize),
        cookies_(Cookies), additional_headers_(AdditionalHeaders)
    {}

public:
    std::string     url() const { return url_; };
    std::string     cookies() const { return cookies_; };
    std::string     additional_headers() const { return additional_headers_; };
    std::string     content_type() const { return content_type_; }
    boost::uint64_t content_size() const { return content_size_; }

private:
    std::string     url_;
    std::string     content_type_;
    boost::uint64_t content_size_;

    std::string     cookies_;
    std::string     additional_headers_;

};

/// \class media_download_list::Impl
class media_download_list::Impl
{
public:
    
    Impl(const std::wstring & Title,
        const std::string & TitleUtf8,
        BytesArrayPtr Thumbnail,
        boost::uint32_t Duration,
        const std::string & Url,
        subtitles_ptr _subs = subtitles_ptr()) :
      title_(Title), title_utf8_(TitleUtf8), thumbnail_(Thumbnail), duration_(Duration), url_(Url), subs_(_subs)
      {}

    size_t size() const 
    {
        return media_download_info_array_.size();
    }

    const media_download_info & media(size_t Index) const
    {
        return media_download_info_array_[Index];
    }

    int media(media_quality_type_t Quality, media_content_type_t Format) const
    {
        std::vector<int> score(media_download_info_array_.size());
        for (size_t j = 0; j < media_download_info_array_.size(); ++j)
        {
            if (media_download_info_array_[j].quality() == Quality && 
                media_download_info_array_[j].content_type() == Format
                )
                return j;
            else
            {
                if (media_download_info_array_[j].content_type() == Format)
                    score[j] = 50 - (std::abs)(media_download_info_array_[j].quality() - Quality);
                else
                    score[j] = 25 - (std::abs)(media_download_info_array_[j].quality() - Quality);
            }
        }

        int maxScore = -1;
        int maxIndex = -1;
        for (size_t i = 0; i < media_download_info_array_.size(); ++i)
        {
            if (score[i] > maxScore)
            {
                maxIndex = i;
                maxScore = score[i];
            }
        }

        return maxIndex;
    }

    std::vector<media_download_info> & get_array() 
    {
        return media_download_info_array_;
    }

    std::wstring            title() const { return title_; }
    std::string             title_utf8() const { return title_utf8_; }
    BytesArrayPtr           thumbnail() const { return thumbnail_; }
    boost::uint32_t         duration() const { return duration_; }
    std::string             url() const 
    { 
        return url_;
    }

    subtitles_ptr subs() const
    {
        return subs_;
    }

private:
    std::vector< media_download_info > media_download_info_array_;
    
    std::wstring  title_;    
    std::string   title_utf8_;    
    BytesArrayPtr thumbnail_;
    boost::uint32_t duration_;    
    std::string url_;
    subtitles_ptr subs_;
    
};

inline std::pair<bool, media_download_info> parse_video(const std::string & url, 
                                const std::string & cookies,
                                media_quality_type_t quality,
                                media_video_type_t video,
                                media_audio_type_t audio)
{
    std::string headers;
    download_page_header_ref(url, headers, cookies, "", "");
    std::vector<HttpHeader> paresed_headers;
    parse_http_headers(headers, paresed_headers); 
    
    std::string media_type;
    boost::uint64_t content_size;

    if (is_media_content(paresed_headers, media_type, content_size))
    {
        media_url_handle_ptr urlH =
            media_url_handle_ptr( new media_url_handle(url, media_type, content_size, cookies, "") );
        media_content_type_t media_type_ = ::openmedia::downloader::parse_media_content_type(media_type);

        media_info_handle_ptr mediaH = media_info_handle_ptr( 
            new media_info_handle(
            0, 
            0, 
            quality,
            media_type_,
            video, 
            audio) );    
        return std::pair<bool, media_download_info>(true, media_download_info(urlH, mediaH));
    }
    return std::pair<bool, media_download_info>(false, media_download_info());
}

} }

#endif // #ifndef _DT_MEDIADOWNLOADERDETAILS_H_

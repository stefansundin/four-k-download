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


#ifndef _DTMEDIAFILE_H_INCLUDED_
#define _DTMEDIAFILE_H_INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

#include <openmedia/DTCommon.h>
#include <openmedia/DTTypes.h>
#include <openmedia/DTDeclareImpl.h>
#include <openmedia/DTPixFormatTypes.h>
#include <openmedia/DTCodecTypes.h>
#include <openmedia/DTSampleFormatTypes.h>

namespace openmedia {

class media_handle;
typedef boost::shared_ptr<media_handle> media_handle_ptr;

class video_data;
typedef boost::shared_ptr<video_data> video_data_ptr;

class file_info_general;
typedef boost::shared_ptr<file_info_general> file_info_general_ptr;

class metadata;
typedef boost::shared_ptr<metadata> metadata_ptr;

class audio_stream_info;
typedef boost::shared_ptr<audio_stream_info> audio_stream_info_ptr;

/// \class  audio_stream_info
class _OPENMEDIASDK_API audio_stream_info : interface_base
{
    DT_DECLARE_IMPL2(audio_stream_info);
public:
    dt_ts_t             get_first_dts() const;
    dt_rational_t       get_time_base() const;
    dt_ts_t             get_start_time() const;
    dt_duration_t       get_duration() const;
    std::string         get_language() const;
    dt_count_t          get_frames_count() const;
    metadata_ptr        get_metadata() const;

    std::string         get_codec_name() const;
    dt_codec_id_t       get_codec_id() const;
    int                 get_bit_rate() const;

    int                 get_sample_rate() const;
    int                 get_channels_count() const;
    dt_sample_format_t  get_sample_format() const;
    dt_channel_layout_t get_channel_layout() const;

    virtual ~audio_stream_info();

protected:
    audio_stream_info(Impl * _Impl);

private:
    Impl * m_pImpl;
};

class video_stream_info;
typedef boost::shared_ptr<video_stream_info> video_stream_info_ptr;

/// \class  video_stream_info
class _OPENMEDIASDK_API video_stream_info : interface_base
{
    DT_DECLARE_IMPL2(video_stream_info);
public:
    dt_rational_t       get_frame_rate() const;
    dt_ts_t             get_first_dts() const;
    dt_rational_t       get_time_base() const;
    dt_ts_t             get_start_time() const;
    dt_duration_t       get_duration() const;
    std::string         get_language() const;
    dt_count_t          get_frames_count() const;
    metadata_ptr        get_metadata() const;
    dt_rational_t       get_avg_frame_rate() const;

    std::string         get_codec_name() const;
    dt_codec_id_t       get_codec_id() const;
    int                 get_bit_rate() const;

    int                 get_width() const;
    int                 get_height() const;
    dt_pixel_format_t   get_pix_fmt() const;
    dt_rational_t       get_sample_aspect_ratio() const;

    virtual ~video_stream_info();

protected:
    video_stream_info(Impl * _Impl);

private:
    Impl * m_pImpl;
};

class media_file_info;
typedef boost::shared_ptr<media_file_info> media_file_info_ptr;

/// \class  media_file_info
class _OPENMEDIASDK_API media_file_info : interface_base
{
    DT_DECLARE_IMPL2(media_file_info);
public:
    file_info_general_ptr   get_file_info() const;
    size_t                  get_video_streams_count() const;
    size_t                  get_audio_streams_count() const;
    audio_stream_info_ptr   get_audio_stream_info(size_t _Index) const;
    video_stream_info_ptr   get_video_stream_info(size_t _Index) const;

    media_handle_ptr get_media_handle() const;
    
    virtual ~media_file_info();

protected:
    media_file_info(Impl * _Impl);

private:
    Impl * m_pImpl;
};

class media_file 
{
public:

    enum OpenResult 
    {
        openSuccess,
        openFail
    };

    typedef boost::function< void (OpenResult /*result*/, media_handle_ptr /*mediaHandle*/) > OnOpen;

    static void open_async(const std::string & fileName, OnOpen onOpen);

    typedef boost::function< void (OpenResult /*result*/, video_data_ptr /*videoData*/) > OnReceiveThumbnail;

    /// \param      PixelFormat     DT_PIX_FMT_NONE : original format
    /// \param      Width           0: no resize
    /// \param      Heigth          0: proportional
    static void receive_thumbnail_async(
        media_handle_ptr mediaHandle,
        OnReceiveThumbnail onReceiveThumbnail,
        int VideoStream = -1,
        dt_pixel_format_t PixelFormat = DT_PIX_FMT_RGB24,
        int Width = 0,
        int Height = 0);

    static media_file_info_ptr get_media_file_info(media_handle_ptr mediaHandle);

};

}

#endif

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


#include "DTHeadersMedia.h"

#include <boost/thread.hpp>

#include <openmedia/DTMediaFile.h>
#include "DTMediaHandle.h"
#include "DTLog.h"
#include <openmedia/DTError.h>
#include <openmedia/DTMediaSplitter.h>
#include <openmedia/DTPictureConvert.h>
#include <openmedia/DTFileInfo.h>
#include <openmedia/DTMediaTypes.h>
#include <openmedia/DTStreamInfo.h>
#include <openmedia/DTVideoDecoderInfo.h>
#include <openmedia/DTVideoDecoder.h>
#include <openmedia/DTString.h>
#include <openmedia/DTPacket.h>
#include <openmedia/DTVideoData.h>
#include <openmedia/DTAudioData.h>

#include "DTMediaFileInfoImpl.h"

namespace openmedia {

namespace {

template <class Str>
void do_open_media_file(Str fileName, media_file::OnOpen onOpen)
{
    try
    {
        media_splitter_ptr mediaSplitter = media_splitter_creator::create(fileName.c_str());
        if (onOpen)
            onOpen(media_file::openSuccess, media_handle::create(mediaSplitter) );
    }
    catch(errors::dt_error &)
    {
        if (onOpen)
            onOpen(media_file::openFail, media_handle_ptr() );
    }
    catch (...)
    {
        if (onOpen)
            onOpen(media_file::openFail, media_handle_ptr() );    
    }    
}


void do_get_thumbnail(media_handle_ptr mediaHandle, 
                      int VideoStream,
                      dt_pixel_format_t pixFormat,
                      int width,
                      int height,
                      media_file::OnReceiveThumbnail onReceiveThumbnail)
{
    bool error = true;

    do 
    {
        if (!mediaHandle || !mediaHandle->get_media_splitter())
        {
            DT_LOG(error) << "[ERROR] [do_get_thumbnail] invalid media handle\n";
            break;
        }

        try
        {
            media_splitter_ptr splitter = mediaHandle->get_media_splitter();

            const unsigned int streams = splitter->get_file_info()->get_streams_count();
            int vidStrm = -1;
            int vidStrmCount = 0;
            for (unsigned int s = 0; s < streams; ++s)
            {
                stream_info_ptr streamInfo = splitter->get_stream_info(s);
                if (DT_AVMEDIA_TYPE_VIDEO == streamInfo->get_decoder_info()->get_codec_type())
                {
                    if (-1 == VideoStream || VideoStream == vidStrmCount)
                    {
                        vidStrm = s; 
                        break;
                    }
                    vidStrmCount++;
                }
            }

            if (-1 == vidStrm)
            {
                DT_LOG(error) << "[ERROR] [do_get_thumbnail] could't find video stream\n";
                break;        
            }

            decoder_info_ptr decInfo = splitter->get_stream_info(vidStrm)->get_decoder_info();
            video_decoder_info * vidDecInfo = dynamic_cast<video_decoder_info *>(decInfo.get());
            DT_ASSERT(NULL != vidDecInfo);

            if (!vidDecInfo)
            {
                DT_LOG(error) << "[ERROR] [do_get_thumbnail] internal error\n";
                break;
            }

            video_decoder_ptr decoder = video_decoder_creator::create(vidDecInfo);

            const dt_ts_t duration = splitter->get_file_info()->get_duration();
            const dt_ts_t startTime = splitter->get_file_info()->get_start_time();
            const dt_rational_t timeBase =  splitter->get_file_info()->get_time_base();

            static const double minDuration = 1.0; // sec

            if (timeBase.den > 0.0 && duration * timeBase.to_double() > minDuration )
            {
                try
                {
                    if (startTime >= 0 && duration >= 0)
                    {
                        const dt_ts_t seekPos = ( startTime + duration ) / 2;
                        splitter->seek(seekPos);
                    }
                }
                catch(errors::dt_error &)
                {
                    // ignore seek
                }
            }


            for (;;)
            {
                media_packet_ptr mediaPacket = splitter->read_packet();
                if (!mediaPacket)
                {
                    if (splitter->is_eof())
                    {
                        DT_LOG(error) << "[ERROR] [do_get_thumbnail] unexpected end of file\n";
                        break;
                    }
                    continue;
                }

                if (vidStrm != mediaPacket->get_stream_index())
                    continue;

                video_data_ptr videoData = decoder->decode(mediaPacket);

                if (!videoData)
                    continue;

                if (0 == videoData->get_height() ||
                    0 == videoData->get_width())
                {
                    DT_LOG(error) << "[ERROR] [do_get_thumbnail] invalid video data size\n";                
                    break;
                }

                if (videoData && videoData->get_data())
                {
                    if (0 == width && 0 == height && DT_PIX_FMT_NONE == pixFormat)
                    {
                        if (onReceiveThumbnail)
                            onReceiveThumbnail(media_file::openSuccess, videoData);
                        error = false;
                        break;
                    }
                    else
                    {
                        double scale = 1.0;
                        if (0 == width && 0 != height)
                            scale = (double)videoData->get_height() / height;
                        else if (0 != width && 0 == height)
                            scale = (double)videoData->get_width() / width;
                        else
                            scale = -1.0;                           

                        int newWidth = videoData->get_width(), newHeight = videoData->get_height(); 
                        if (scale > 0.0)
                        {
                            newWidth = static_cast<int>(videoData->get_width() / scale);
                            newHeight = static_cast<int>(videoData->get_height() / scale);
                        }
                        
                        picture_convert_ptr convert = picture_convert_creator::create(
                            videoData->get_width(), videoData->get_height(), videoData->get_pixel_format(),
                            newWidth, newHeight, pixFormat, DT_SWS_BICUBIC);

                        video_data_ptr convertedFrame = convert->convert(videoData);

                        if (!convertedFrame)
                        {
                            DT_LOG(error) << "[ERROR] [do_get_thumbnail] invalid converted frame\n";
                            break;
                        }

                        if (onReceiveThumbnail)
                            onReceiveThumbnail(media_file::openSuccess, convertedFrame);

                        error = false;
                        break;
                    }
                } 
                else if (splitter->is_eof())
                {
                    DT_LOG(error) << "[ERROR] [do_get_thumbnail] internal error\n";
                    break;
                }
            }
        } 
        catch (errors::dt_error &)
        {
            DT_LOG(error) << "[ERROR] [do_get_thumbnail] internal exception\n";
            break;
        }
        catch(...)
        {
            DT_LOG(error) << "[ERROR] [do_get_thumbnail] unexpected exception\n";
            break;
        }
    }
    while (0);

    if (error)
    {
        if (onReceiveThumbnail)
            onReceiveThumbnail(media_file::openFail, video_data_ptr() );
    }
}

}

void media_file::open_async(const std::string & fileName, OnOpen onOpen)
{
    boost::thread(&do_open_media_file<std::string>, fileName, onOpen);
}

void media_file::receive_thumbnail_async(
    media_handle_ptr mediaHandle,
    OnReceiveThumbnail onReceiveThumbnail,
    int VideoStream,
    dt_pixel_format_t PixelFormat,
    int Width,
    int Height)
{
    boost::thread(&do_get_thumbnail, mediaHandle,
        VideoStream,
        PixelFormat,
        Width,
        Height,
        onReceiveThumbnail);
}

file_info_general_ptr media_file_info::get_file_info() const
{
    return impl()->get_file_info();
}

size_t media_file_info::get_video_streams_count() const
{
    return impl()->get_video_streams_count();
}

size_t media_file_info::get_audio_streams_count() const
{
    return impl()->get_audio_streams_count();
}

audio_stream_info_ptr media_file_info::get_audio_stream_info(size_t _Index) const
{
    return impl()->get_audio_stream_info(_Index);
}

video_stream_info_ptr media_file_info::get_video_stream_info(size_t _Index) const
{
    return impl()->get_video_stream_info(_Index);
}

media_handle_ptr media_file_info::get_media_handle() const
{
    return impl()->get_media_handle();
}

media_file_info::media_file_info( media_file_info::Impl * _Impl ) : m_pImpl(_Impl)
{
    DT_STRONG_ASSERT(NULL != _Impl);
}

media_file_info::~media_file_info()
{
    delete m_pImpl;
}

class media_file_info_ : public media_file_info
{
public:
    media_file_info_(media_handle_ptr mediaHandle) : media_file_info( new media_file_info::Impl(mediaHandle) )
    {        
    }
};

media_file_info_ptr media_file::get_media_file_info(media_handle_ptr mediaHandle)
{
    return media_file_info_ptr( new media_file_info_(mediaHandle) );
}

}

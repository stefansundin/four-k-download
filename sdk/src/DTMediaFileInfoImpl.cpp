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

#include "DTMediaFileInfoImpl.h"
#include "DTMediaHandle.h"

#include <openmedia/DTStreamInfo.h>
#include <openmedia/DTFileInfo.h>
#include <openmedia/DTMediaSplitter.h>

namespace openmedia {

media_file_info::Impl::Impl(media_handle_ptr Handle) : handle_(Handle), videoStreamsCount_(0), audioStreamsCount_(0)
{
    if (media_splitter_ptr splitter = Handle->get_media_splitter())
    {
        if (file_info_general_ptr fileInfo = splitter->get_file_info())
        {
            const unsigned int strmsCnt = fileInfo->get_streams_count();
            for (unsigned int i = 0; i < strmsCnt; ++i)
            {
                switch (splitter->get_stream_info(i)->get_decoder_info()->get_codec_type())
                {
                case DT_AVMEDIA_TYPE_VIDEO:
                    videoStreamsCount_++;
                    break;
                case DT_AVMEDIA_TYPE_AUDIO:
                    audioStreamsCount_++;
                    break;
                default:
                    break;
                }
            }
        }
    }
}

file_info_general_ptr media_file_info::Impl::get_file_info() const
{
    if (media_handle_ptr handle = handle_.lock())
    {
        if (media_splitter_ptr splitter = handle->get_media_splitter())
            return splitter->get_file_info();    
    }
    return file_info_general_ptr();
}

size_t media_file_info::Impl::get_video_streams_count() const
{
    return videoStreamsCount_;
}

size_t media_file_info::Impl::get_audio_streams_count() const
{
    return audioStreamsCount_;   
}

audio_stream_info_ptr media_file_info::Impl::get_audio_stream_info(size_t _Index) const
{
    return audio_stream_info_ptr(); // TODO
}

video_stream_info_ptr media_file_info::Impl::get_video_stream_info(size_t _Index) const
{
    return video_stream_info_ptr(); // TODO
}

media_handle_ptr media_file_info::Impl::get_media_handle() const
{
    return handle_.lock();
}

}

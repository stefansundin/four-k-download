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


#ifndef _DTMEDIAFILEINFO_IMPL_H_INCLUDED_
#define _DTMEDIAFILEINFO_IMPL_H_INCLUDED_

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <openmedia/DTMediaFile.h>

namespace openmedia {

class media_file_info::Impl
{
public:
    Impl(media_handle_ptr Handle);

public:
    file_info_general_ptr   get_file_info() const;
    size_t                  get_video_streams_count() const;
    size_t                  get_audio_streams_count() const;
    audio_stream_info_ptr   get_audio_stream_info(size_t _Index) const;
    video_stream_info_ptr   get_video_stream_info(size_t _Index) const;

    media_handle_ptr        get_media_handle() const;

private:
    boost::weak_ptr<media_handle> handle_;
    unsigned int videoStreamsCount_;
    unsigned int audioStreamsCount_;

};

}

#endif

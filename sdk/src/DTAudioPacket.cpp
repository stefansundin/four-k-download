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
#include "DTHeadersMedia.h"
// precompiled header end

/// \file   DTAudioData.cpp

#include <openmedia/DTAudioData.h>
#include <openmedia/DTPacket.h>
#include "DTPacketImpl.h"

namespace openmedia {

class audio_packet_impl : public media_packet::Impl
{
public:
    audio_packet_impl(audio_data_ptr _AudioData);

public:
    virtual dt_ts_t             get_pts() const { return 0; }
    virtual dt_ts_t             get_dts() const { return 0; }
    virtual const uint8_t *     get_data() const;
    virtual dt_data_size_t      get_size() const;
    virtual int                 get_stream_index() const { return -1; }
    virtual int                 get_flags() const { return 0; }
    virtual dt_media_data_duration_t get_duration() const ;
    virtual dt_filesize_t       get_byte_pos() const { return 0; }
    virtual dt_media_type_t     get_media_type() const;
    virtual bool                is_valid() const { return true; }
    virtual media_packet_ptr    duplicate() const;
    virtual dt_rational_t       get_time_base() const;
    virtual dt_rational_t       get_frame_rate() const;
    
    virtual ~audio_packet_impl();

private:
    audio_data_ptr audioData_;

};

audio_packet_impl::audio_packet_impl(audio_data_ptr _AudioData) : audioData_(_AudioData)
{}

const uint8_t * audio_packet_impl::get_data() const
{
    return audioData_->get_raw_data();
}

dt_data_size_t      audio_packet_impl::get_size() const
{
    return audioData_->get_raw_size();
}

dt_media_data_duration_t audio_packet_impl::get_duration() const
{
    return 0;
}

dt_media_type_t audio_packet_impl::get_media_type() const
{
    return DT_AVMEDIA_TYPE_AUDIO;
}

media_packet_ptr audio_packet_impl::duplicate() const
{
    return audio_data::to_packet(audioData_);
}

dt_rational_t audio_packet_impl::get_time_base() const
{
    return dt_rational_t();
}

dt_rational_t audio_packet_impl::get_frame_rate() const
{
    return dt_rational_t();
}

audio_packet_impl::~audio_packet_impl()
{}

media_packet_ptr audio_data::to_packet(audio_data_ptr _AudioData)
{
    std::auto_ptr<audio_packet_impl> audio_packet_impl_(new audio_packet_impl(_AudioData));
    media_packet_ptr media_packet_ = media_packet_ptr( new media_packet( audio_packet_impl_.get() ) );
    audio_packet_impl_.release();
    return media_packet_;
}

} // namespace openmedia

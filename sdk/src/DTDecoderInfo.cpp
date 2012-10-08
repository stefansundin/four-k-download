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

/// \file   DTDecoderInfo.cpp

#include <openmedia/DTDecoderInfo.h>
#include "DTDecoderInfoImpl.h"

namespace openmedia {

const uint8_t * decoder_info::get_extradata() const
{
    return impl()->get_extradata();
}

int decoder_info::get_extradata_size() const
{
    return impl()->get_extradata_size();
}

std::string decoder_info::get_codec_name() const
{
    return impl()->get_codec_name();
}

dt_codec_base_t decoder_info::get_codec_base() const
{
    return impl()->get_codec_base();
}

dt_media_type_t decoder_info::get_codec_type() const
{
    return impl()->get_codec_type();
}

dt_codec_id_t decoder_info::get_codec_id() const
{
    return impl()->get_codec_id();
}

unsigned int decoder_info::get_codec_tag() const
{
    return impl()->get_codec_tag();
}

int decoder_info::get_profile() const
{
    return impl()->get_profile();
}

int decoder_info::get_level() const
{
    return impl()->get_level();
}

int decoder_info::get_bit_rate() const
{
    return impl()->get_bit_rate();
}

dt_rational_t decoder_info::get_time_base() const
{
    return impl()->get_time_base();
}

int decoder_info::get_rc_max_rate() const
{
    return impl()->get_rc_max_rate();
}

int decoder_info::get_rc_buffer_size() const
{
    return impl()->get_rc_buffer_size();
}

///////////////////////////////////////

decoder_info::~decoder_info()
{
    delete m_pImpl;
    m_pImpl = NULL;
}

decoder_info::decoder_info(decoder_info::Impl * _Impl) : m_pImpl(_Impl)
{
    DT_STRONG_ASSERT(NULL != _Impl);
}

/////////////////////

std::string video_decoder_additional_info::get_codec_name() const
{
    return impl()->get_codec_long_name();
}

std::string video_decoder_additional_info::get_codec_long_name() const
{
    return impl()->get_codec_long_name();
}

video_decoder_additional_info::~video_decoder_additional_info()
{
    delete m_pImpl;
}

video_decoder_additional_info::video_decoder_additional_info(Impl * _Impl) : m_pImpl(_Impl)
{
    DT_STRONG_ASSERT( NULL != _Impl );
}

};

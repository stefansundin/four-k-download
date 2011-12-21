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


/// \file   DTDecoderInfo.h

#ifndef _DTDECODERINFOIMPL_H_INCLUDED_
#define _DTDECODERINFOIMPL_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <openmedia/DTCommon.h>
#include <openmedia/DTDecoderInfo.h>
#include <openmedia/DTVideoDecoderInfo.h>
#include <openmedia/DTAudioDecoderInfo.h>

namespace openmedia {

/// \class  decoder_info::Impl
class decoder_info::Impl: impl_base
{
public:
    virtual const uint8_t * get_extradata() const = 0;
    virtual int             get_extradata_size() const = 0;
    virtual std::string     get_codec_name() const = 0;
    virtual dt_codec_base_t get_codec_base() const = 0;
    virtual dt_media_type_t get_codec_type() const = 0;
    virtual dt_codec_id_t   get_codec_id() const = 0;
    virtual unsigned int    get_codec_tag() const = 0;
    virtual int             get_profile() const = 0;
    virtual int             get_level() const = 0;
    virtual int             get_bit_rate() const = 0;
    virtual dt_rational_t   get_time_base() const = 0;
    virtual int             get_rc_max_rate() const = 0;
    virtual int             get_rc_buffer_size() const = 0;  

};

/// \class  video_decoder_additional_info::Impl
class video_decoder_additional_info::Impl : public impl_base
{
public:
    virtual std::string get_codec_name() const = 0;
    virtual std::string get_codec_long_name() const = 0;

};

} // namespace openmedia 

#endif

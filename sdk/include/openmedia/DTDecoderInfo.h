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

#ifndef _DTDECODERINFO_H_INCLUDED_
#define _DTDECODERINFO_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <openmedia/DTCommon.h>
#include <openmedia/DTDeclareImpl.h>
#include <openmedia/DTConfig.h>
#include <openmedia/DTTypes.h>
#include <openmedia/DTCodecTypes.h>
#include <openmedia/DTMediaTypes.h>

namespace openmedia {

class decoder_info;
typedef boost::shared_ptr<decoder_info>          decoder_info_ptr;

/// \class  decoder_info
class _OPENMEDIASDK_API decoder_info: interface_base
{
    DT_DECLARE_IMPL2(decoder_info);
public:
    const uint8_t * get_extradata() const;
    int             get_extradata_size() const;
    std::string     get_codec_name() const;
    dt_codec_base_t get_codec_base() const;
    dt_media_type_t get_codec_type() const;
    dt_codec_id_t   get_codec_id() const;
    unsigned int    get_codec_tag() const;
    int             get_profile() const;
    int             get_level() const;
    int             get_bit_rate() const;
    dt_rational_t   get_time_base() const;
    int             get_rc_max_rate() const;
    int             get_rc_buffer_size() const;    
    virtual         ~decoder_info() = 0;

protected:
    decoder_info(decoder_info::Impl * _Impl);

private:
    decoder_info::Impl * m_pImpl;

};

}

#endif

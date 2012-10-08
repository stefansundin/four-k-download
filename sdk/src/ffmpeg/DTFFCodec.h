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


#ifndef _DTFFCODEC_H_INCLUDED_
#define _DTFFCODEC_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <string>

#include <boost/shared_ptr.hpp>

#include <openmedia/DTCommon.h>
#include <openmedia/DTCodecTypes.h>

struct AVCodecContext;

namespace openmedia {
class additional_settings;
typedef boost::shared_ptr<AVCodecContext> AVCodecContextPtr;

AVCodecContextPtr dt_create_codec_context();
AVCodecContextPtr dt_create_codec_context(AVCodecContext * _AVCodecContext);
void dt_open_ff_decoder(AVCodecContext * _AVCodecContext, dt_codec_id_t _CodecID);

//void AVCodecContextAdditionalSettings(AVCodecContext * _CodecContext, const additional_settings * _AdditionalSettings);

bool get_ff_codec_name_by_id(dt_codec_id_t _CodecId, std::string & _Name, std::string & _LongName);

} // namespace openmedia

#endif // #ifndef _DTFFCODEC_H_INCLUDED_

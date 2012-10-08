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


/// \file   DTVideoDecoder.h

#ifndef _DTVIDEODECODER_H_INCLUDED_
#define _DTVIDEODECODER_H_INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <openmedia/DTConfig.h>
#include <openmedia/DTCommon.h>
#include <openmedia/DTDeclareImpl.h>
#include <openmedia/DTError.h>

#include <openmedia/DTDecoder.h>
#include <openmedia/DTDecoderInfo.h>
#include <openmedia/DTAdditionalSettings.h>

namespace openmedia {

class media_packet;
typedef boost::shared_ptr<media_packet> media_packet_ptr;

class video_data;
typedef boost::shared_ptr<video_data> video_data_ptr;

template <class MediaData> class media_data_timed;
typedef media_data_timed<video_data> video_data_timed;
typedef boost::shared_ptr<video_data_timed> video_data_timed_ptr;

class video_decoder_additional_info;
typedef boost::shared_ptr<video_decoder_additional_info> video_decoder_additional_info_ptr;

class video_decoder_info;
typedef boost::shared_ptr<video_decoder_info> video_decoder_info_ptr;

class video_decoder;
typedef boost::shared_ptr<video_decoder> video_decoder_ptr;

/// \class video_decoder
class _OPENMEDIASDK_API video_decoder: public decoder
{
    DT_DECLARE_IMPL2(video_decoder);
public:

public:
    video_data_ptr          decode(media_packet_ptr _MediaPacket);
    video_data_timed_ptr    decode_timed(media_packet_ptr _MediaPacket);
    video_decoder_info_ptr  get_decoder_info() const;
    video_decoder_additional_info_ptr get_additional_info() const;

protected:
    video_decoder(video_decoder::Impl * _Impl);

private:
    video_decoder::Impl * m_pImpl;
    
};

class video_decoder_info;
class additional_settings;

class _OPENMEDIASDK_API video_decoder_creator
{
public:
    static video_decoder_ptr create(video_decoder_info * _DecoderInfo);
    static video_decoder_ptr create(video_decoder_info * _DecoderInfo, const additional_settings * _AdditionalSettings);
};

template <>
class MediaDecoderSelector<video_data>
{
public:
    typedef video_decoder MediaDecoder;
};

} // namespace openmedia 

#endif

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


/// \file   DTAudioDecoder.h

#ifndef _DTAUDIODECODER_H_INCLUDED_
#define _DTAUDIODECODER_H_INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>


#include <openmedia/DTCommon.h>
#include <openmedia/DTDeclareImpl.h>
#include <openmedia/DTError.h>

#include <openmedia/DTDecoder.h>

namespace openmedia {

class audio_decoder_additional_info;
typedef boost::shared_ptr<audio_decoder_additional_info> audio_decoder_additional_info_ptr;

class media_packet;
typedef boost::shared_ptr<media_packet> media_packet_ptr;

class audio_data;
typedef boost::shared_ptr<audio_data> audio_data_ptr;

template <class MediaData> class media_data_timed;
typedef media_data_timed<audio_data> audio_data_timed;
typedef boost::shared_ptr<audio_data_timed> audio_data_timed_ptr;

class audio_decoder_info;
typedef boost::shared_ptr<audio_decoder_info> audio_decoder_info_ptr;

/// \class  audio_decoder_additional_info
class audio_decoder_additional_info : interface_base
{
    DT_DECLARE_IMPL2(audio_decoder_additional_info);
public:
    std::string get_codec_name() const;
    std::string get_codec_long_name() const;
    
    virtual ~audio_decoder_additional_info();

protected:
    audio_decoder_additional_info(Impl * _Impl);

private:
    Impl * m_pImpl;

};

class audio_decoder;
typedef boost::shared_ptr<audio_decoder> audio_decoder_ptr;

/// \class  audio_decoder
class _OPENMEDIASDK_API audio_decoder : public decoder
{
    DT_DECLARE_IMPL2(audio_decoder);
public:
    audio_data_ptr                      decode(media_packet_ptr _MediaPacket);
    audio_data_timed_ptr                decode_timed(media_packet_ptr _MediaPacket);
    audio_decoder_info_ptr              get_decoder_info() const;
    audio_decoder_additional_info_ptr   get_additional_info() const;

public:
    static audio_decoder_ptr create(const audio_decoder_info * _DecoderInfo);
    
protected:
    audio_decoder(audio_decoder::Impl * _Impl);

private:
    audio_decoder::Impl * m_pImpl;
    
};

template <>
class MediaDecoderSelector<audio_data>
{
public:
    typedef audio_decoder MediaDecoder;
};


} // namespace openmedia 

#endif

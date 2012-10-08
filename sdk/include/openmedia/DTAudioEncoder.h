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


/// \file   DTAudioEncoder.h

#ifndef _DTAUDIOENCODER_H_INCLUDED_
#define _DTAUDIOENCODER_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/shared_ptr.hpp>

#include <openmedia/DTConfig.h>
#include <openmedia/DTCommon.h>
#include <openmedia/DTCStdInt.h>

#include <openmedia/DTEncoderInfo.h>

#include <openmedia/DTAudioEncoderDirect.h>
#include <openmedia/DTAudioEncoderQueue.h>


namespace openmedia {

class media_settings;
typedef boost::shared_ptr<media_settings> media_settings_ptr;

class audio_format;

class audio_encoder_settings;
typedef boost::shared_ptr<audio_encoder_settings> audio_encoder_settings_ptr;

/// \class  audio_encoder_settings
class _OPENMEDIASDK_API audio_encoder_settings : interface_base
{
    DT_DECLARE_IMPL2(audio_encoder_settings);
public:
    EncoderMode     get_mode() const;
    int             get_bit_rate() const;
    double          get_quality() const;
    
    virtual         ~audio_encoder_settings();
    
    audio_encoder_settings_ptr duplicate() const;
    audio_encoder_settings_ptr clone() const;

public:
    static audio_encoder_settings_ptr create_bitrate_base(int BitRate);
    static audio_encoder_settings_ptr create_quality_base(double Quality);

protected:
    audio_encoder_settings(Impl * _Impl);

private:
    Impl * m_pImpl;
};


class _OPENMEDIASDK_API audio_encoder_creator {
public:
//    static audio_encoder_ptr create_lame_mp3_encoder(const audio_format * _InputAudioFormat);
//    static audio_encoder_ptr create_lame_mp3_encoder(const audio_format * _InputAudioFormat, const char * Artist, const char * Title);
    static audio_encoder_queue_ptr create_lame_mp3_encoder_queue(const audio_format * _InputAudioFormat);
    static audio_encoder_queue_ptr create_lame_mp3_encoder_queue(const audio_format * _InputAudioFormat, const char * Artist, const char * Title, int Bitrate = -1);
    
    static audio_encoder_queue_ptr create(
        const std::string& formatName,
        const audio_format * inputAudioFormat,
        media_settings_ptr settings);

};

} // namespace openmedia

#endif // #ifndef _DTAUDIOENCODER_H_INCLUDED_


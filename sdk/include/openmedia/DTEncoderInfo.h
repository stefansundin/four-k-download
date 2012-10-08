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


#ifndef _DTENCODERINFO_H_INCLUDED_
#define _DTENCODERINFO_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <openmedia/DTCStdInt.h>
#include <openmedia/DTCommon.h>
#include <openmedia/DTDecoderInfo.h>

namespace openmedia {

enum EncoderMode
{
   encoderBitrateBase = 0,
   encoderQualityBase = 1
};

#if 0

class encoder_settings;
typedef boost::shared_ptr<encoder_settings> encoder_settings_ptr;

class _OPENMEDIASDK_API media_settings : interface_base
{
    DT_DECLARE_IMPL2(encoder_settings);
public:
    int                 get_bit_rate() const; ///< the average bitrate, unused for constant quantizer encoding.
    int                 get_bit_rate_tolerance() const; ///< number of bits the bitstream is allowed to diverge from the reference
    const uint8_t *     get_extradata() const;
    int                 get_extradata_size() const;
    dt_rational_t       get_time_base() const;

protected:
    encoder_settings(Impl * _Impl);

private:
    Impl * m_pImpl;

};

class encoder_info;
class video_encoder_info;
class audio_encoder_info;



/// \class  encoder_info
class _OPENMEDIASDK_API encoder_info : interface_base
{
    DT_DECLARE_IMPL2(encoder_info);
public:
    int                 get_bit_rate() const; ///< the average bitrate, unused for constant quantizer encoding.
    int                 get_bit_rate_tolerance() const; ///< number of bits the bitstream is allowed to diverge from the reference
    int                 get_codec_flags() const;
    const uint8_t *     get_extradata() const;
    int                 get_extradata_size() const;
    dt_rational_t       get_time_base() const;

protected:
    encoder_info(Impl * _Impl);

private:
    Impl * m_pImpl;

};

/// \class  video_encoder_info
class _OPENMEDIASDK_API video_encoder_info : public encoder_info
{
    DT_DECLARE_IMPL2(video_encoder_info);
public:
    MotionEstimation    get_motion_estimation() const;
    int                 get_gop_size() const;
    dt_pixel_format_t   get_pix_fmt() const;
    int                 get_rate_emu() const; // \TODO ?
    int                 get_qmin() const;
    int                 get_qmax() const;
    int                 get_max_qdiff() const;
    int                 get_max_b_frames() const;
    int                 get_b_quant_factor() const;
    

protected:
    video_encoder_info(Impl * _Impl);

private:
    Impl * m_pImpl;

};
#endif

} // namespace openmedia

#endif // #ifndef _DTENCODERINFO_H_INCLUDED_


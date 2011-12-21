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


/// \file   DTVideoDecoderInfo.h

#ifndef _DTVIDEODECODERINFO_H_INCLUDED_
#define _DTVIDEODECODERINFO_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <openmedia/DTDecoderInfo.h>
#include <openmedia/DTPixFormatTypes.h>

namespace openmedia {

class video_decoder_info;
typedef boost::shared_ptr<video_decoder_info>    video_decoder_info_ptr;

/// \class  video_decoder_info
class _OPENMEDIASDK_API video_decoder_info: public decoder_info
{
    DT_DECLARE_IMPL2(video_decoder_info);
public:
    int                 get_width() const;
    int                 get_height() const;
    dt_pixel_format_t   get_pix_fmt() const;
    bool                has_b_frames() const;
    dt_rational_t       get_sample_aspect_ratio() const;
    const uint16_t *    get_intra_matrix() const;
    const uint16_t *    get_inter_matrix() const;
    int                 get_reference_frames() const; ///< ?
    int                 get_ticks_per_frame() const;

protected:
    video_decoder_info(video_decoder_info::Impl * _Impl);

private:
    video_decoder_info::Impl * m_pImpl;

};

/// \class  audio_decoder_additional_info
class video_decoder_additional_info : public interface_base
{
    DT_DECLARE_IMPL2(video_decoder_additional_info);
public:
    std::string get_codec_name() const;
    std::string get_codec_long_name() const;
    
    virtual ~video_decoder_additional_info();

protected:
    video_decoder_additional_info(Impl * _Impl);

private:
    Impl * m_pImpl;

};

}

#endif

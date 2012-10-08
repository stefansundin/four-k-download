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

/// \file   DTVideoDecoderInfo.cpp

#include <openmedia/DTVideoDecoderInfo.h>
#include "DTVideoDecoderInfoImpl.h"

namespace openmedia {

int video_decoder_info::get_width() const
{
    return impl()->get_width();
}

int video_decoder_info::get_height() const
{
    return impl()->get_height();
}

dt_pixel_format_t video_decoder_info::get_pix_fmt() const
{
    return impl()->get_pix_fmt();
}

bool video_decoder_info::has_b_frames() const
{
    return impl()->has_b_frames();
}

dt_rational_t video_decoder_info::get_sample_aspect_ratio() const
{
    return impl()->get_sample_aspect_ratio();
}

const uint16_t * video_decoder_info::get_intra_matrix() const
{
    return impl()->get_intra_matrix();
}

const uint16_t * video_decoder_info::get_inter_matrix() const
{
    return impl()->get_inter_matrix();
}

int video_decoder_info::get_reference_frames() const
{
    return impl()->get_reference_frames();
}

int video_decoder_info::get_ticks_per_frame() const
{
    return impl()->get_ticks_per_frame();
}

///////////////////////////////////////

video_decoder_info::video_decoder_info(video_decoder_info::Impl * _Impl): decoder_info(_Impl), m_pImpl(_Impl)
{
}

}

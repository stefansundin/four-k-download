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


/// \file   DTVideoDecoderInfoImpl.h

#ifndef _DTVIDEODECODERINFOIMPL_H_INCLUDED_
#define _DTVIDEODECODERINFOIMPL_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <openmedia/DTDecoderInfo.h>
#include "DTDecoderInfoImpl.h"

namespace openmedia {

/// \class  video_decoder_info::Impl
class video_decoder_info::Impl: public decoder_info::Impl
{
public:
    virtual int                 get_width() const = 0;
    virtual int                 get_height() const = 0;
    virtual dt_pixel_format_t   get_pix_fmt() const = 0;
    virtual bool                has_b_frames() const = 0;
    virtual dt_rational_t       get_sample_aspect_ratio() const = 0;
    virtual const uint16_t *    get_intra_matrix() const = 0;
    virtual const uint16_t *    get_inter_matrix() const = 0;
    virtual int                 get_reference_frames() const = 0; ///< ?
    virtual int                 get_ticks_per_frame() const = 0;

};

}

#endif

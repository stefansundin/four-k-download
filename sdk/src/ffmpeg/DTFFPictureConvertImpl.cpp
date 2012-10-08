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

#include <boost/make_shared.hpp>

#include "DTFFPictureConvertImpl.h"
#include "DTFFPicture.h"
#include "DTFFVideoData.h"

using namespace boost;

namespace openmedia {

namespace details {

inline bool same_picture_format(int _NewWidth, int _NewHeight, dt_pixel_format_t _NewPixelFormat, int _OldWidth, int _OldHeight, dt_pixel_format_t _OldPixelFormat)
{
    return  _NewWidth == _OldWidth &&
            _NewHeight == _OldHeight &&
            _NewPixelFormat == _OldPixelFormat;
}

} // namespace details

swscale_picture_convert_impl::swscale_picture_convert_impl(int inWidth,
                                                           int inHeight,
                                                           dt_pixel_format_t inPixFmt,
                                                           int outWidth,
                                                           int outHeight,
                                                           dt_pixel_format_t outPixFmt,
                                                           int _Flags) : 
outputWidth_(outWidth),
outputHeight_(outHeight),
outputPixelFormat_(DT2FFType(outPixFmt)),
inputWidth_ (inWidth),
inputHeight_(inHeight),
inputPixelFormat_(DT2FFType(inPixFmt)),
swsContext_ ( sws_getContext (inWidth, inHeight, DT2FFType(inPixFmt),
             outWidth, outHeight, DT2FFType(outPixFmt), _Flags, NULL, NULL, NULL),
             &sws_freeContext),
             outputFrame_( dt_create_picture(outPixFmt, outWidth, outHeight) )
{
    if (!swsContext_)
    {
        BOOST_THROW_EXCEPTION( picture_convert::error() << boost::errinfo_api_function("sws_getContext") );
    }
}

video_data_ptr swscale_picture_convert_impl::convert(video_data_ptr inData)
{
    if (!swsContext_)
    {
        BOOST_THROW_EXCEPTION( errors::unexpected() );
        DT_IF_DISABLE_EXCEPTIONS(return video_data_ptr());
    }

    uint8_t * data[4] = {
        inData->get_plane(0),
        inData->get_plane(1),
        inData->get_plane(2),
        inData->get_plane(3)
    };

    int linesize[4] = {
        inData->get_line_size(0),
        inData->get_line_size(1),
        inData->get_line_size(2),
        inData->get_line_size(3)
    };

    sws_scale(swsContext_.get(), data, linesize, 0, inputHeight_, outputFrame_.get()->data, outputFrame_.get()->linesize);

    return make_shared<ff_video_data>(outputFrame_, outputWidth_, outputHeight_, FF2DTType(outputPixelFormat_));
    
}


} // namespace openmedia


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

#include <openmedia/DTVideoEncoder.h>
#include <string>
#include <vector>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include "DTVideoEncoderFactory.h"
#include <openmedia/DTMediaSettings.h>

namespace openmedia {

namespace 
{

video_encoder_queue_ptr create_impl(const std::string & FormatName, media_settings_ptr VideoEncoderSettings)
{
    std::vector<std::string> result;
    std::string fmt(FormatName);
    boost::algorithm::split(result, fmt, boost::algorithm::is_any_of("/"));

    if ( !result.size() )
    {
        BOOST_THROW_EXCEPTION( errors::invalid_operation() );            
        DT_IF_DISABLE_EXCEPTIONS(return video_encoder_queue_ptr());
    }

    if (result[0] == "openmedia")
    {
        if (result.size() < 2)
        {
            BOOST_THROW_EXCEPTION( errors::invalid_operation() );            
            DT_IF_DISABLE_EXCEPTIONS(return video_encoder_queue_ptr());
        }

        return video_encoder_factory::get()->create(result[1], VideoEncoderSettings);
    }
    
    BOOST_THROW_EXCEPTION( errors::invalid_operation() );            
    DT_IF_DISABLE_EXCEPTIONS(return video_encoder_queue_ptr());    
}

}

video_encoder_queue_ptr video_encoder_creator::create(const char * FormatName, media_settings_ptr VideoEncoderSettings)
{
    return create_impl(FormatName, VideoEncoderSettings);
}

video_encoder_queue_ptr video_encoder_creator::create(const std::string& FormatName, media_settings_ptr VideoEncoderSettings)
{
    return create(FormatName.c_str(), VideoEncoderSettings);
}


video_encoder_queue_ptr video_encoder_creator::create(media_settings_ptr VideoEncoderSettings)
{
    return create_impl(VideoEncoderSettings->get_param("video_encoder"), VideoEncoderSettings);
}

video_encoder_queue_ptr video_encoder_creator::create(const char * FormatName)
{
    return create_impl(FormatName, media_settings_ptr());
}

}

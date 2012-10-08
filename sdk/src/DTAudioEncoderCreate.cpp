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

#include <vector>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <openmedia/DTAudioEncoder.h>
#include <openmedia/DTError.h>
#include <openmedia/DTMediaSettings.h>
#include <openmedia/DTAudioFormat.h>
#include "DTAudioEncoderFactory.h"

namespace openmedia {

namespace 
{
audio_encoder_queue_ptr create_impl(const std::string& formatName,
    const audio_format * inputAudioFormat,
    media_settings_ptr settings)
{
    std::vector<std::string> result;
    std::string fmt(formatName);
    boost::algorithm::split(result, fmt, boost::algorithm::is_any_of("/"));
    if ( !result.size() )
    {
        BOOST_THROW_EXCEPTION( errors::invalid_operation() );            
        DT_IF_DISABLE_EXCEPTIONS(return audio_encoder_queue_ptr());
    }

    if (result[0] == "openmedia")
    {
        if (result.size() < 2)
        {
            BOOST_THROW_EXCEPTION( errors::invalid_operation() );            
            DT_IF_DISABLE_EXCEPTIONS(return audio_encoder_queue_ptr());
        }

        media_settings_ptr applySettings = media_settings::create(settings);
        media_settings_ptr audioSettings = audio_format::serialize(inputAudioFormat);
        applySettings->apply_from(audioSettings);
        
        return audio_encoder_factory::get()->create(result[1], applySettings);
    }

    BOOST_THROW_EXCEPTION(errors::invalid_operation());
    DT_IF_DISABLE_EXCEPTIONS(return audio_encoder_queue_ptr());
}

}

audio_encoder_queue_ptr audio_encoder_creator::create(
    const std::string& formatName,
    const audio_format * inputAudioFormat,
    media_settings_ptr settings)
{
    return create_impl(formatName, inputAudioFormat, settings);
}

}

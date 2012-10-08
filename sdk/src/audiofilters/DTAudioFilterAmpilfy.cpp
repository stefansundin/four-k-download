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


#include "DTHeadersMedia.h"

#include "../DTAudioFilterImpl.h"
#include <openmedia/DTAudioFilters.h>
#include <openmedia/DTAudioConvert.h>
#include <openmedia/DTMediaSettings.h>

namespace openmedia
{

class audio_filter_amplify_impl : public audio_filter::Impl
{
public:
    audio_filter_amplify_impl(const audio_format * audioFormat) : gain_(1.0)
    {
        inFormat_ = audioFormat->clone();        
        workFormat_ = audio_format::create(inFormat_->get_sample_rate(),
            DT_SAMPLE_FMT_FLT,
            inFormat_->get_channels_count(),
            inFormat_->get_channel_layout());
        inToFloat_ = audio_convert_utils::create_resample_convert(workFormat_.get(), inFormat_.get()); 
        floatToIn_ = audio_convert_utils::create_resample_convert(inFormat_.get(), workFormat_.get());
    }

private:
    void transform()
    {
        audio_data_ptr data = inToFloat_->pop_front(inToFloat_->get_samples_count());
        
        uint8_t * d = (data->lock_data());

        const int chCount = data->get_channels_count();
        const int sCount = data->get_samples_count();
        const ptrdiff_t diff = data->get_sample_align();
        
        for(int s = 0; s < sCount; ++s)
        {
            float * df = reinterpret_cast<float*>(d);
            for (int c = 0; c < chCount; ++c)
            {
                *df = *df * gain_;
                if (*df > 1.0)
                    *df = 1.0;
                else if (*df < -1.0)
                    *df = -1.0;
                df++;
            }
            
            d += diff;            
        }
        data->unlock_data();
        floatToIn_->push_back(data.get());    
    }


public:
    virtual void push_back(const audio_data * audioData)
    {
        inToFloat_->push_back(audioData);
        transform();
    }

    virtual audio_data_ptr      pop_front(int samplesCount)
    {
        return floatToIn_->pop_front(samplesCount);    
    }

    virtual void push_back_raw(const uint8_t * data, size_t dataSize)
    {
        inToFloat_->push_back_raw(data, dataSize);
        transform();
    }

    virtual audio_format_ptr get_output_audio_format() const
    {
        return floatToIn_->get_output_audio_format();
    }

    virtual int get_samples_count() const
    {
        return floatToIn_->get_samples_count();
    }

public:
    virtual void apply(media_settings * settings)
    {
        if (settings)
            gain_ = settings->get_param<double>("gain");
    }

private:
    audio_convert_ptr inToFloat_;
    audio_convert_ptr floatToIn_;
    audio_format_ptr inFormat_;
    audio_format_ptr workFormat_;
    double gain_;
};

class audio_filter_amplify : public audio_filter
{
public:
    audio_filter_amplify(const audio_format * audioFormat) : audio_filter(new audio_filter_amplify_impl(audioFormat))
    {}

};

audio_filter_ptr audio_filters_utils::create_amplify(const audio_format * audioFormat)
{
    return audio_filter_ptr( new audio_filter_amplify(audioFormat) );
}

}

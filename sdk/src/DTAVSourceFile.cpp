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

#include "DTAVSourceFile.h"
#include "DTAVSourceImpl.h"
#include <boost/scoped_ptr.hpp>
#include <openmedia/DTVideoSource.h>
#include <openmedia/DTAudioSource.h>

#include <openmedia/DTVideoFormat.h>
#include <openmedia/DTPacket.h>
#include <openmedia/DTAudioFormat.h>
#include <openmedia/DTAudioDecoder.h>
#include <openmedia/DTVideoDecoder.h>
#include <openmedia/DTVideoDecoderInfo.h>
#include <openmedia/DTAudioDecoderInfo.h>
#include <openmedia/DTStreamInfo.h>
#include <openmedia/DTFileInfo.h>
#include <openmedia/DTPictureConvert.h>
#include <openmedia/DTAudioConvert.h>
#include <openmedia/DTMediaSplitter.h>
#include <openmedia/DTVideoDataTimed.h>
#include <openmedia/DTAudioData.h>
#include <openmedia/utils/DTConcurrentQueue.h>

namespace openmedia {

class av_video_file_impl;

/// \class  class av_video_file_video_source
class av_video_file_video_source : public video_source
{
public:
    av_video_file_video_source(av_video_file_impl * owner) : owner_(owner) {}
public:
    virtual double get_duration() const;
    virtual double get_current_pos() const;
    virtual double skip(double duration);
    virtual video_data_timed_ptr read_video();
    virtual video_format_ptr get_video_format() const;
    virtual double get_frame_rate() const;
    virtual bool enabled() const;
    virtual void enabled(bool val);
private:
    av_video_file_impl * owner_;
    
};

/// \class  av_video_file_audio_source
class av_video_file_audio_source : public audio_source
{
public:
    av_video_file_audio_source(av_video_file_impl * owner) : owner_(owner) {}
public:
    virtual double get_duration() const;
    virtual double get_current_pos() const;
    virtual double skip(double duration);
    virtual audio_data_timed_ptr read_audio(double duration);
    virtual audio_format_ptr get_audio_format() const;
    virtual bool enabled() const;
    virtual void enabled(bool val);
private:
    av_video_file_impl * owner_;
};

class av_video_file_impl : public av_source::Impl
{
    friend class av_video_file_video_source;
    friend class av_video_file_audio_source;
public:
    av_video_file_impl(const char * fileName);

public:
    virtual video_source * video();
    virtual audio_source * audio();
    virtual double seek(double position);
    virtual void apply_settings(media_settings_ptr /*settings*/) {};

    virtual ~av_video_file_impl();

private:
    bool read_next_packet();
    
    template <class Queue>
    media_packet_ptr read_packet(Queue & q);

    media_packet_ptr read_audio_packet();
    media_packet_ptr read_video_packet();

    double get_video_duration() const;
    double get_audio_duration() const;

private:
    media_splitter_ptr splitter_;
    video_format_ptr outputVideoFormat_;
    audio_format_ptr outputAudioFormat_;
    audio_decoder_ptr audioDecoder_;
    video_decoder_ptr videoDecoder_;
    picture_convert_ptr pictureConverter_;
    audio_convert_ptr audioConverter_;
    int audioStream_;
    int videoStream_;

    boost::scoped_ptr<av_video_file_video_source> video_;
    boost::scoped_ptr<av_video_file_audio_source> audio_;

    utils::concurrent_queue<media_packet_ptr> video_packets_;
    utils::concurrent_queue<media_packet_ptr> audio_packets_;

    double videoTime_;
    double audioTime_;

    bool videoEnabled_;
    bool audioEnabled_;

    double videoDuration_;
    double audioDuration_;

    double videoFrameRate_;

    bool eof_;
};

double av_video_file_impl::get_video_duration() const
{
    return videoDuration_;    
}

double av_video_file_impl::get_audio_duration() const
{
    return audioDuration_;
}

av_video_file_impl::av_video_file_impl(const char * fileName): 
audioStream_(-1),
videoStream_(-1),
videoTime_(0),
audioTime_(0),
videoEnabled_(false),
audioEnabled_(false),
videoDuration_(0.0),
audioDuration_(0.0),
eof_(false)
{
    const int videoWidth = 640;
    const dt_pixel_format_t videoPixelFormat = DT_PIX_FMT_RGB24;
    
    const int audioSampleRate = 44100;
    const dt_sample_format_t audioSampleFormat = DT_SAMPLE_FMT_S16;
    const int audioChannelsCount = 2;
    const dt_channel_layout_t audioChannelLayout = 3;

    ///

    media_splitter_ptr splitter = media_splitter_creator::create(fileName);
    const unsigned int streams = splitter->get_file_info()->get_streams_count();
    int vs = -1, as = -1;
    for (unsigned int s = 0; s < streams; ++s)
    {
        stream_info_ptr streamInfo = splitter->get_stream_info(s);
        if (DT_AVMEDIA_TYPE_VIDEO == streamInfo->get_decoder_info()->get_codec_type() && vs == -1)
            vs = s; 
        
        if (DT_AVMEDIA_TYPE_AUDIO == streamInfo->get_decoder_info()->get_codec_type() && as == -1)
            as = s; 
    }

    splitter_ = splitter;

    if (vs >= 0)
    {
        decoder_info_ptr decoderInfo = splitter->get_stream_info(vs)->get_decoder_info();    
        double frameRate = splitter->get_stream_info(vs)->get_frame_rate().to_double();
        video_decoder_info * videoDecoderInfo = dynamic_cast<video_decoder_info *>(decoderInfo.get());
        DT_ASSERT(NULL != videoDecoderInfo);
        video_decoder_ptr videoDecoder = video_decoder_creator::create(videoDecoderInfo);

        video_format_ptr outputVideoFormat 
            = video_format::create(videoWidth,
            (1.0 * videoWidth / videoDecoderInfo->get_width() ) * videoDecoderInfo->get_height(),
            videoPixelFormat
            );
        
        picture_convert_ptr converter = 
            picture_convert_creator::create(videoDecoderInfo->get_width(), videoDecoderInfo->get_height(), videoDecoderInfo->get_pix_fmt(),
            outputVideoFormat->get_width(), outputVideoFormat->get_height(), outputVideoFormat->get_pix_fmt(), DT_SWS_BICUBIC);

        const dt_duration_t duration = splitter->get_stream_info(vs)->get_duration();
        const dt_rational_t timeBase = splitter->get_stream_info(vs)->get_time_base();        
        videoDuration_ =  1000.0 * timeBase.to_double() * duration;

        videoFrameRate_ = frameRate;
        videoDecoder_ = videoDecoder;
        pictureConverter_ = converter;
        outputVideoFormat_ = outputVideoFormat;
        videoStream_ = vs;
        video_.reset(new av_video_file_video_source(this));
        videoEnabled_ = true;
    }

    if (as >= 0)
    {
        decoder_info_ptr decoderInfo = splitter->get_stream_info(as)->get_decoder_info();    
        audio_decoder_info * audioDecoderInfo = dynamic_cast<audio_decoder_info *>(decoderInfo.get());
        DT_ASSERT(NULL != audioDecoderInfo);
        audio_decoder_ptr audioDecoder = audio_decoder::create(audioDecoderInfo);

        audio_format_ptr inputAudioFormat = audio_format::create(
            audioDecoderInfo->get_sample_rate(),
            audioDecoderInfo->get_sample_format(),
            audioDecoderInfo->get_channels_count(),
            audioDecoderInfo->get_channel_layout());

        audio_format_ptr outputAudioFormat = audio_format::create(
            audioSampleRate,
            audioSampleFormat,
            audioChannelsCount,
            audioChannelLayout);

        audio_convert_ptr audioConverter = 
            audio_convert_utils::create_resample_convert(outputAudioFormat.get(),
            inputAudioFormat.get());

        const dt_duration_t duration = splitter->get_stream_info(as)->get_duration();
        const dt_rational_t timeBase = splitter->get_stream_info(as)->get_time_base();        
        audioDuration_ =  1000.0 * timeBase.to_double() * duration;

        audioDecoder_ = audioDecoder;
        audioConverter_ = audioConverter;
        outputAudioFormat_ = outputAudioFormat;
        audioStream_ = as;
        audio_.reset(new av_video_file_audio_source(this));
        audioEnabled_ = true;
    }
    eof_ = false;
}

bool av_video_file_impl::read_next_packet()
{
    if (eof_)
        return false;

    media_packet_ptr packet = splitter_->read_packet();
    if (!packet)
    {
        eof_ = true;
        return false;
    }

    if (videoStream_ == packet->get_stream_index() && videoEnabled_)
        video_packets_.push(packet);
    else if (audioStream_ == packet->get_stream_index() && audioEnabled_)
        audio_packets_.push(packet);

    return true;
}

template <class Queue>
media_packet_ptr av_video_file_impl::read_packet(Queue & q)
{
    for (;;)
    {
        if (!q.empty())
        {
            media_packet_ptr packet;
            q.wait_and_pop(packet);    
            return packet;
        }
        if (!read_next_packet())
            return media_packet_ptr();
    }
}

media_packet_ptr av_video_file_impl::read_video_packet()
{
    return read_packet(video_packets_);
}

media_packet_ptr av_video_file_impl::read_audio_packet()
{
    return read_packet(audio_packets_);
}

video_source * av_video_file_impl::video()
{
    return video_.get();
}

audio_source * av_video_file_impl::audio()
{
    return audio_.get();
}

double av_video_file_impl::seek(double position)
{
    video_packets_.clear();
    audio_packets_.clear();
    audioConverter_->pop_front( audioConverter_->get_samples_count() );
    dt_rational_t timeBase = splitter_->get_file_info()->get_time_base();
    
    const dt_ts_t ts = (position / 1000.0) * timeBase.den / timeBase.num;
    splitter_->seek(ts);
    audioTime_ = position;
    eof_ = false;
    return position;
}

av_video_file_impl::~av_video_file_impl()
{}

///////////////////////////////

double av_video_file_video_source::get_current_pos() const
{
    return owner_->videoTime_;
}

double av_video_file_video_source::skip(double duration)
{
    double readed = 0.0;
    while (readed < duration)
    {
        video_data_timed_ptr vd = this->read_video();
        if (vd)
            readed += vd->get_duration();
        else
            break;
    }
    return get_current_pos();
}

video_data_timed_ptr av_video_file_video_source::read_video()
{
    DT_STRONG_ASSERT(NULL != owner_);
    
    if (!owner_->videoEnabled_)
        return video_data_timed_ptr();

    for (;;)
    {
        media_packet_ptr packet = owner_->read_video_packet();
        if (!packet)
            return video_data_timed_ptr();
        
        if (video_data_timed_ptr videoData = owner_->videoDecoder_->decode_timed(packet))
        {
            video_data_ptr videoDataConv = owner_->pictureConverter_->convert(videoData->get_media_data());
            return video_data_timed::create(videoDataConv, videoData->get_duration(), owner_->videoTime_ = videoData->get_time_stamp());
        }
    }
}

video_format_ptr av_video_file_video_source::get_video_format() const
{
    return owner_->outputVideoFormat_;
}

bool av_video_file_video_source::enabled() const
{
    return owner_->videoEnabled_;
}

void av_video_file_video_source::enabled(bool val)
{
    if (!owner_->videoEnabled_ && true == val)
    {
        owner_->videoDecoder_->reset();
        owner_->video_packets_.clear();
    }

    owner_->videoEnabled_ = val;
}

double av_video_file_video_source::get_frame_rate() const
{
    return owner_->videoFrameRate_;
}

double av_video_file_video_source::get_duration() const
{
    return owner_->get_video_duration();
}

////////////////////

double av_video_file_audio_source::get_current_pos() const
{
    return owner_->audioTime_;
}

double av_video_file_audio_source::skip(double duration)
{
    audio_data_timed_ptr resultTimed = read_audio(duration);
    return owner_->audioTime_;
}

audio_data_timed_ptr av_video_file_audio_source::read_audio(double duration)
{
    DT_STRONG_ASSERT(NULL != owner_);

    if (!owner_->audioEnabled_)
        return audio_data_timed_ptr();

    const int samplesRequired 
        = static_cast<int>(0.5 + duration / 1000.0 * owner_->outputAudioFormat_->get_sample_rate());

    while (samplesRequired > owner_->audioConverter_->get_samples_count())
    {
        if (media_packet_ptr packet = owner_->read_audio_packet())
        {
            audio_data_timed_ptr audioData = owner_->audioDecoder_->decode_timed(packet);
            if (audioData && audioData->get_media_data()->get_samples_count())
                owner_->audioConverter_->push_back(audioData->get_media_data().get());            
        }
        else
            break;
    }

    if (const size_t samples = (std::min)(owner_->audioConverter_->get_samples_count(), samplesRequired))
    {
        audio_data_ptr result = owner_->audioConverter_->pop_front(samplesRequired);
        const double realDuration = 1000.0 * result->get_samples_count() / owner_->outputAudioFormat_->get_sample_rate();
        audio_data_timed_ptr resultTimed = audio_data_timed::create(result, realDuration, owner_->audioTime_);
        owner_->audioTime_ += realDuration;
        return resultTimed;
    }
    else
        return audio_data_timed_ptr();
}

audio_format_ptr av_video_file_audio_source::get_audio_format() const
{
    DT_STRONG_ASSERT(NULL != owner_);
    return owner_->outputAudioFormat_;
}

bool av_video_file_audio_source::enabled() const
{
    return owner_->audioEnabled_;
}

void av_video_file_audio_source::enabled(bool val)
{
    if (!owner_->audioEnabled_ && true == val)
    {
        owner_->audio_packets_.clear();
        owner_->audioDecoder_->reset();                
    }
    owner_->audioEnabled_ = val;
}

double av_video_file_audio_source::get_duration() const
{
    return owner_->get_audio_duration();
}

////
av_video_file::av_video_file(const char *fileName): 
av_source( new av_video_file_impl(fileName) )
{
}

}

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

#include <iostream>

#define BOOST_THREAD_USE_LIB
#include <boost/thread.hpp>
#include <boost/make_shared.hpp>

#include <openmedia/DTVideo2Mp3.h>

#include <openmedia/DTMediaSplitter.h>
#include <openmedia/DTFileInfo.h>
#include <openmedia/DTStreamInfo.h>
#include <openmedia/DTAudioDecoder.h>
#include <openmedia/DTAudioDecoderInfo.h>
#include <openmedia/DTAudioFormat.h>
#include <openmedia/DTMP3File.h>
#include <openmedia/DTAudioEncoder.h>
#include <openmedia/DTAudioConvert.h>
#include <openmedia/DTString.h>
#include <openmedia/DTAudioData.h>

#include "DTMediaHandle.h"

namespace openmedia {
namespace audio { 

using ::openmedia::utf8_to_utf16;
using ::openmedia::utf16_to_utf8;
using ::openmedia::ansi_to_utf8;
namespace {

void convert_video_to_mp3_impl_splitter(media_splitter_ptr splitter,
                               std::string OutputMp3Name,
                               std::wstring Title,
                               video2mp3::ConvertStateNotify StateNotify,
                               video2mp3::GetControl OnGetControl, bool UseSourceBitrate);

void convert_video_to_mp3_impl_handle(media_handle_ptr MediaHandle,
                               std::string OutputMp3Name,
                               std::wstring Title,
                               video2mp3::ConvertStateNotify StateNotify,
                               video2mp3::GetControl OnGetControl, bool UseSourceBitrate);

void convert_video_to_mp3_impl(std::string InputFileName,
                               std::string OutputMp3Name,
                               std::wstring Title,
                               video2mp3::ConvertStateNotify StateNotify, bool UseSourceBitrate)
{
    media_splitter_ptr splitter;
    try 
    {
        splitter = media_splitter_creator::create(InputFileName.c_str());
    }
    catch(...)
    {
        StateNotify(video2mp3::stateError, 1.0);
        return;
    }

    return convert_video_to_mp3_impl_splitter(splitter,
        OutputMp3Name,
        Title,
        StateNotify,
        video2mp3::GetControl(),
        UseSourceBitrate);
}

void convert_video_to_mp3_impl_handle(media_handle_ptr MediaHandle,
                               std::string OutputMp3Name,
                               std::wstring Title,
                               video2mp3::ConvertStateNotify StateNotify,
                               video2mp3::GetControl OnGetControl,
                               bool UseSourceBitrate)
{
    DT_ASSERT(!!MediaHandle);
    if (!MediaHandle)
    {
        StateNotify(video2mp3::stateError, 1.0);
        return;
    }

    media_splitter_ptr splitter;
    try
    {
        splitter = MediaHandle->get_media_splitter();
        if (splitter)
        {
            splitter->seek(0);
        }
        else
        {
            StateNotify(video2mp3::stateError, 1.0);
            return;
        }
    }
    catch(...)
    {
        StateNotify(video2mp3::stateError, 1.0);
        return;
    }

    return convert_video_to_mp3_impl_splitter(splitter,
                                     OutputMp3Name,
                                     Title,
                                     StateNotify,
                                     OnGetControl,
                                     UseSourceBitrate);
}

namespace {

class video2mp3_control_impl : public video2mp3_control
{
public:
    typedef boost::function< void (video2mp3::State) > ChangeState;

    video2mp3_control_impl(ChangeState OnChangeState) : paused_(false),
        cancel_(false),
        on_change_state_(OnChangeState) {}

    virtual void pause()
    {
        boost::upgrade_lock<boost::shared_mutex> lock(lock_);

        if (!paused_)
        {
            boost::upgrade_to_unique_lock<boost::shared_mutex> lock1(lock);
            paused_ = true;
            notify();
        }
    }

    virtual void resume()
    {
        boost::upgrade_lock<boost::shared_mutex> lock(lock_);

        if (paused_)
        {
            boost::upgrade_to_unique_lock<boost::shared_mutex> lock1(lock);
            paused_ = false;
            notify();
        }
    }

    virtual void cancel()
    {
        boost::unique_lock<boost::shared_mutex> lock(lock1_);
        cancel_ = true;
    }

    void notify()
    {
        if (on_change_state_)
        {
            if (paused_)
                on_change_state_(video2mp3::statePause);
            else
                on_change_state_(video2mp3::stateResume);
        }
    }

    bool isPaused() const
    {
        boost::shared_lock<boost::shared_mutex> lock(lock_);
        return paused_;
    }

    bool isCancel() const
    {
        boost::shared_lock<boost::shared_mutex> lock(lock1_);
        return cancel_;
    }

private:
    bool paused_;
    bool cancel_;
    ChangeState on_change_state_;
    mutable boost::shared_mutex lock_;
    mutable boost::shared_mutex lock1_;

};


class StateNotifyHelper
{
public:
    StateNotifyHelper(video2mp3::ConvertStateNotify StateNotify) :
        StateNotify_(StateNotify)
    {
    }

    void operator()(video2mp3::State Val)
    {
        if (StateNotify_)
            StateNotify_(Val, 0.0);
    }
private:
    video2mp3::ConvertStateNotify StateNotify_;
};

}

namespace {
int normalize_bitrate(int bitrate)
{
    if (bitrate <= 129000)
        return 128;
    else if (bitrate <= 193000)
        return 192;
    else if (bitrate <= 257000)
        return 256;
    else
        return 320;
}
}

void convert_video_to_mp3_impl_splitter(media_splitter_ptr splitter,
                               std::string OutputMp3Name,
                               std::wstring Title,
                               video2mp3::ConvertStateNotify StateNotify,
                               video2mp3::GetControl OnGetControl, bool UseSourceBitrate)
{
    try
    {

        boost::shared_ptr<video2mp3_control_impl> control = boost::make_shared<video2mp3_control_impl>( StateNotifyHelper(StateNotify) );
        if (OnGetControl)
            OnGetControl(control);

        std::string title = utf16_to_utf8(Title);

        int inputBitrate = -1;
        const unsigned int streamsCount = splitter->get_file_info()->get_streams_count();
        int firstAudioStreamIndex = -1;
        for (unsigned int s = 0; s < streamsCount; ++s)
        {
            stream_info_ptr streamInfo = splitter->get_stream_info(s);
            if (firstAudioStreamIndex == -1 && openmedia::DT_AVMEDIA_TYPE_AUDIO == streamInfo->get_decoder_info()->get_codec_type())
            {
                if (decoder_info_ptr decoderInfo = streamInfo->get_decoder_info())
                {
                    inputBitrate = normalize_bitrate(decoderInfo->get_bit_rate());
                }

                firstAudioStreamIndex = s;
                break;
            }
        }

        if (firstAudioStreamIndex == -1)
        {
            // no audio
            StateNotify(video2mp3::stateError, 1.0);
            return;
        }

        double duration = 0.0;
        if (splitter->get_file_info()->get_time_base().den != 0)
        {
            duration = splitter->get_file_info()->get_duration() *
                splitter->get_file_info()->get_time_base().num / splitter->get_file_info()->get_time_base().den;
        }

        decoder_info_ptr decoderInfo = splitter->get_stream_info(firstAudioStreamIndex)->get_decoder_info();
        audio_decoder_info * audioDecoderInfo = dynamic_cast<audio_decoder_info *>(decoderInfo.get());

        audio_decoder_ptr audioDecoder = audio_decoder::create(audioDecoderInfo);

        audio_format_ptr outputAudioFormat = audio_format::create(
            44100,
            DT_SAMPLE_FMT_S16,
            2,
            3
            );


        int outputBitrate = UseSourceBitrate ? inputBitrate : -1;
        audio_encoder_queue_ptr audioEncoder = audio_encoder_creator::create_lame_mp3_encoder_queue(outputAudioFormat.get(), "", (char*)title.c_str(), outputBitrate);

        media_muxer_mp3file mp3File(OutputMp3Name.c_str(), outputAudioFormat.get(), audioEncoder->get_extra_data() );

        audio_format_ptr inputAudioFormat =
            audio_format::create(audioDecoderInfo->get_sample_rate(),
            audioDecoderInfo->get_sample_format(),
            audioDecoderInfo->get_channels_count());

        audio_convert_ptr audioConvertSampleRate =
            audio_convert_utils::create_resample_convert(outputAudioFormat.get(),
            inputAudioFormat.get());

        size_t packetCount = 0;

        for (;;)
        {
            while (control->isPaused())
            {
                boost::this_thread::sleep( boost::posix_time::millisec(200) );
                if (control->isCancel())
                    break;
            }

            if (control->isCancel())
                break;

            media_packet_ptr packet = splitter->read_packet();

            bool bEnd = false;

            if (!packet)
            {
                if (splitter->is_eof())
                {
                    audioConvertSampleRate->push_back(NULL);
                    audio_data_ptr convertedData = audioConvertSampleRate->pop_front(audioConvertSampleRate->get_samples_count());
                    if (convertedData)
                        audioEncoder->send_audio(convertedData.get());
                    audioEncoder->flush();
                }
                bEnd = true;
            }
            else
            {
                if (firstAudioStreamIndex == packet->get_stream_index())
                {
                    if (!( (++packetCount) % 50) && packet->get_time_base().den)
                    {
                        double persent = 0.5;
                        if (duration > 0.0)
                            persent =(( 1.0 * packet->get_dts() * packet->get_time_base().num / packet->get_time_base().den ) / duration );

                        if (!StateNotify(video2mp3::stateProgress, persent))
                        {
                            StateNotify(video2mp3::stateFinish, persent);
                            return;
                        };
                    }

                    audio_data_ptr audioData = audioDecoder->decode(packet);

                    if (!!audioData && audioData->get_samples_count())
                    {
                        audioConvertSampleRate->push_back(audioData.get());
                        audio_data_ptr convertedData = audioConvertSampleRate->pop_front(audioConvertSampleRate->get_samples_count());

                        if (!!convertedData)
                        {
                            audioEncoder->send_audio( convertedData.get() );
                        }
                    }
                }
            }

            media_packet_ptr encodedData;
            do
            {
                encodedData = audioEncoder->receive_packet( );
                if (encodedData )
                {
                    mp3File.write_packet( encodedData );
                }
            }
            while(!!encodedData);
            if (bEnd)
                break;

        }
    }
    catch(...)
    {
        StateNotify(video2mp3::stateError, 1.0);
        return;
    }

    StateNotify(video2mp3::stateFinish, 1.0);
}

}

void video2mp3::convert(std::string InputFileName,
                        std::string OutputMp3Name,
                        std::wstring Title,
                        ConvertStateNotify StateNotify,
                        bool UseSourceBitrate)
{
    boost::thread convert_thread(&convert_video_to_mp3_impl, 
        InputFileName,
        OutputMp3Name,
        Title,
        StateNotify,
        UseSourceBitrate
        );    
}

void video2mp3::convert(media_handle_ptr MediaHandle,
                        std::string OutputMp3Name,
                        std::wstring Title,
                        ConvertStateNotify StateNotify,
                        video2mp3::GetControl OnGetControl, bool UseSourceBitrate)
{
    boost::thread convert_thread( boost::bind(&convert_video_to_mp3_impl_handle,
        MediaHandle,
        OutputMp3Name,
        Title,
        StateNotify,
        OnGetControl, UseSourceBitrate)
        );
}

void video2mp3::convert(media_handle_ptr MediaHandle, std::string OutputMp3Name, std::wstring Title, ConvertStateNotify StateNotify, bool UseSourceBitrate)
{
    return convert(MediaHandle, OutputMp3Name, Title, StateNotify, GetControl(), UseSourceBitrate);
}


} // namespace openmedia
} // namespace audio

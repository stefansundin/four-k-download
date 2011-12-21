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


#include <openmedia/DTHeaders.h>

#include <iostream>

#define BOOST_THREAD_USE_LIB
#include <boost/thread.hpp>

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

namespace openmedia {
namespace audio { 

using ::openmedia::utf8_to_utf16;
using ::openmedia::utf16_to_utf8;
using ::openmedia::ansi_to_utf8;

namespace {

void convert_video_to_mp3_impl(std::string InputFileName, std::string OutputMp3Name, std::wstring Title, video2mp3::ConvertStateNotify StateNotify)
{
    try 
    {
        ::std::string title = utf16_to_utf8(Title);
        ::std::wstring inputName     = utf8_to_utf16(InputFileName);
        ::std::wstring outputMp3Name    = utf8_to_utf16(OutputMp3Name);
#if defined(DT_CONFIG_HAVE_UTF16_OPEN) && (1 == DT_CONFIG_HAVE_UTF16_OPEN)
        media_splitter_ptr splitter = media_splitter_creator::create(inputName.c_str());
#else
        media_splitter_ptr splitter = media_splitter_creator::create(InputFileName.c_str());
#endif

        const unsigned int streamsCount = splitter->get_file_info()->get_streams_count();
        int firstAudioStreamIndex = -1;
        for (unsigned int s = 0; s < streamsCount; ++s)
        {
            stream_info_ptr streamInfo = splitter->get_stream_info(s);
            if (firstAudioStreamIndex == -1 && openmedia::DT_AVMEDIA_TYPE_AUDIO == streamInfo->get_decoder_info()->get_codec_type())
            {
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
            audioDecoderInfo->get_channels_count(),
            audioDecoderInfo->get_channel_layout()
            );

        audio_encoder_queue_ptr audioEncoder = audio_encoder_utils::create_lame_mp3_encoder_queue(outputAudioFormat.get(), "", (char*)title.c_str());

#if defined(DT_CONFIG_HAVE_UTF16_OPEN) && (1 == DT_CONFIG_HAVE_UTF16_OPEN)
        media_muxer_mp3file mp3File(outputMp3Name.c_str(), outputAudioFormat.get(), audioEncoder->get_extra_data() );
#else
        ::std::string outputMp3NameUtf8 = utf16_to_utf8(outputMp3Name);
        media_muxer_mp3file mp3File(outputMp3NameUtf8.c_str(), outputAudioFormat.get(), audioEncoder->get_extra_data() );
#endif

        audio_convert_ptr audioConvertSampleRate =
            audio_convert_utils::create_resample_convert(outputAudioFormat.get(), 
            audioDecoderInfo->get_sample_rate(),
            outputAudioFormat->get_sample_rate()
            );

        size_t packetCount = 0;

        for (;;)
        {
            media_packet_ptr packet = splitter->read_packet();
            
            bool bEnd = false;
            if (!packet)
            {
                if (splitter->is_eof())
                {
                    audioEncoder->send_audio ( NULL );
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

void video2mp3::convert(std::wstring InputFileName, std::wstring OutputMp3Name, std::wstring Title, ConvertStateNotify StateNotify)
{
    boost::thread convert_thread(&convert_video_to_mp3_impl, 
        utf16_to_utf8(InputFileName),
        utf16_to_utf8(OutputMp3Name),
        Title,
        StateNotify
        );    
}

void video2mp3::convert(std::string InputFileName, std::string OutputMp3Name, std::wstring Title, ConvertStateNotify StateNotify)
{
    std::string inputFlvName, outputMp3Name;

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    inputFlvName = ansi_to_utf8(InputFileName);
    outputMp3Name = ansi_to_utf8(OutputMp3Name);
#else
    inputFlvName = inputFlvName;
    outputMp3Name = OutputMp3Name;
#endif

    boost::thread convert_thread(&convert_video_to_mp3_impl, 
        InputFileName,
        OutputMp3Name,
        Title,
        StateNotify
        );    
}


} // namespace openmedia
} // namespace audio

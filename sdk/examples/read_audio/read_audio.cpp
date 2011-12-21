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


#include <iostream>

#include <boost/shared_ptr.hpp>
#include <boost/timer.hpp>
#include <boost/lexical_cast.hpp>

#include <openmedia/DTMediaSplitter.h>
#include <openmedia/DTFileInfo.h>
#include <openmedia/DTDecoderInfo.h>
#include <openmedia/DTError.h>
#include <openmedia/DTAudioDecoder.h>
#include <openmedia/DTAudioDecoderInfo.h>
#include <openmedia/DTAudioUtils.h>
#include <openmedia/DTStreamInfo.h>
#include <openmedia/DTAudioConvert.h>
#include <openmedia/DTWAVFile.h>
#include <vector>

#ifdef _MSC_VER
#   pragma comment(lib, "dtcommonsdk-static.lib")
#   pragma comment(lib, "dtmediasdk-static.lib")

#   pragma comment(lib, "libavformat.a")
#   pragma comment(lib, "libavcodec.a")
#   pragma comment(lib, "libavutil.a")
#   pragma comment(lib, "libgcc.lib")
#   pragma comment(lib, "libmingwex.lib") 
#   pragma comment(lib, "libmp3lame.a") 
#endif

using namespace openmedia;

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cerr << "usage: test_read_audio.exe [inputfile] [outputfile.wav]\n";
        return 1;
    }

    try
    {
        const char * fileName = argv[1];
        media_splitter_ptr splitter = media_splitter_creator::create(fileName);

        unsigned int streamsCount = splitter->get_file_info()->get_streams_count();
        int firstAudioStreamIndex = -1;
        for (unsigned int s = 0; s < streamsCount; ++s)
        {
            stream_info_ptr streamInfo = splitter->get_stream_info(s);
            if (firstAudioStreamIndex == -1 && openmedia::DT_AVMEDIA_TYPE_AUDIO == streamInfo->get_decoder_info()->get_codec_type())
                firstAudioStreamIndex = s; 
        }

        decoder_info_ptr decoderInfo = splitter->get_stream_info(firstAudioStreamIndex)->get_decoder_info();
        audio_decoder_info * audioDecoderInfo = dynamic_cast<audio_decoder_info *>(decoderInfo.get());
        DT_ASSERT(NULL != audioDecoderInfo);

        audio_decoder_ptr audioDecoder = audio_decoder::create(audioDecoderInfo);

        audio_format_ptr outputAudioFormat = audio_format::create(
            /*audioDecoderInfo->get_sample_rate()*/ 44100,
            DT_SAMPLE_FMT_S16,
            audioDecoderInfo->get_channels_count(),
            audioDecoderInfo->get_channel_layout());

        media_muxer_wavfile waveFile(argv[2], outputAudioFormat.get());

        std::vector<uint8_t> tempBuffer;

        audio_convert_ptr audioConvert = audio_convert_utils::create_sample_fmt_convert(outputAudioFormat.get());
        audio_convert_ptr audioConvertSampleRate =
            audio_convert_utils::create_resample_convert(outputAudioFormat.get(), 
            audioDecoderInfo->get_sample_rate(),
            outputAudioFormat->get_sample_rate()
            );

		uint64_t packetCount = 0;
        boost::timer timerElapsed;
        for (;;)
        {
            media_packet_ptr packet = splitter->read_packet();

            if (!packet)
                break;

            if (firstAudioStreamIndex == packet->get_stream_index())
            {
				packetCount++;
				if (0 == packetCount % 100)
					std::cout << packetCount << ": audio pts: " << packet->get_pts() << "\r";

                audio_data_ptr audioData = audioDecoder->decode(packet);

                if (!!audioData && audioData->get_samples_count())
                {
                    audioConvertSampleRate->push_back(audioData.get());
                    audio_data_ptr convertedData = audioConvertSampleRate->pop_front(audioConvertSampleRate->get_samples_count());

                    if (!! convertedData)
                    {
                        media_packet_ptr packet = audio_data::to_packet(convertedData);
                        waveFile.write_packet(packet);
                    }
                }
            }
        }
    }
    catch(openmedia::errors::dt_error & e)
    {
        std::cerr << "\nDiagnostic information:\n";
        std::cerr << boost::diagnostic_information(e);
    }

    std::cout << "ok";
    return 0;
}


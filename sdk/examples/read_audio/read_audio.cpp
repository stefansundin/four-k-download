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


/// \file   read_audio.cpp

#define BOOST_THREAD_USE_LIB

#include <boost/shared_ptr.hpp>
#include <boost/timer.hpp>
#include <boost/lexical_cast.hpp>

#include <openmedia/DTCommon.h>
#include <openmedia/DTTypes.h>
#include <openmedia/DTError.h>
#include <openmedia/DTMediaSplitter.h>
#include <openmedia/DTDecoderInfo.h>
#include <openmedia/DTAudioDecoder.h>
#include <openmedia/DTAudioDecoderInfo.h>
#include <openmedia/DTStreamInfo.h>
#include <openmedia/DTFileInfo.h>
#include <openmedia/DTWAVFile.h>
#include <openmedia/DTAudioConvert.h>
#include <openmedia/DTAudioFormat.h>
#include <openmedia/DTAudioData.h>
#include <openmedia/DTAudioDataTimed.h>
#include <iostream>
#include <vector>

#ifdef _MSC_VER
#   pragma comment(lib, "dtcommonsdk-static.lib")
#   pragma comment(lib, "dtmediasdk-static.lib")

#   pragma comment(lib, "avcodec.lib")
#   pragma comment(lib, "avformat.lib")
#   pragma comment(lib, "avutil.lib")
#   pragma comment(lib, "swresample.lib")
#   pragma comment(lib, "swscale.lib")
#endif

using namespace openmedia;

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cout << "usage: " << argv[0] << " [inputfile] [outputfile.wav]\n";
        return 1;
    }
    try
    {
        media_splitter_ptr splitter = media_splitter_creator::create(argv[1]);

        const unsigned int strmCnt = splitter->get_file_info()->get_streams_count();
        int audStrm = -1;
        for (unsigned int s = 0; s < strmCnt; ++s)
        {
            stream_info_ptr streamInfo = splitter->get_stream_info(s);
            if (audStrm == -1 && openmedia::DT_AVMEDIA_TYPE_AUDIO == streamInfo->get_decoder_info()->get_codec_type())
                audStrm = s; 
        }

        decoder_info_ptr decoderInfo = splitter->get_stream_info(audStrm)->get_decoder_info();
        audio_decoder_info * audInfo = dynamic_cast<audio_decoder_info *>(decoderInfo.get());
        DT_ASSERT(NULL != audInfo);

        audio_decoder_ptr audioDecoder = audio_decoder::create(audInfo);

        audio_format_ptr outputAudioFormat = audio_format::create(
            44100,
            DT_SAMPLE_FMT_S16,
            audInfo->get_channels_count(),
            audInfo->get_channel_layout());

         audio_format_ptr inputAudioFormat = audio_format::create(
            audInfo->get_sample_rate(),
            audInfo->get_sample_format(),
            audInfo->get_channels_count(),
            audInfo->get_channel_layout());


        media_muxer_wavfile waveFile(argv[2], outputAudioFormat.get());

        std::vector<boost::uint8_t> tempBuffer;

        audio_convert_ptr audioConvert = 
            audio_convert_utils::create_resample_convert(outputAudioFormat.get(),
            inputAudioFormat.get()
            );

		boost::uint64_t packetCount = 0;
        boost::timer timerElapsed;
        for (;;)
        {
            media_packet_ptr packet = splitter->read_packet();

            if (!packet)
                break;

            if (audStrm == packet->get_stream_index())
            {
				packetCount++;
				if (0 == packetCount % 100)
					std::cout << packetCount << ": audio pts: " << packet->get_pts() << "\r";

                audio_data_ptr audioData = audioDecoder->decode(packet);

                if (audioData && audioData->get_samples_count())
                {
                    audioConvert->push_back(audioData.get());
                    if (audio_data_ptr convertedData = audioConvert->pop_front(audioConvert->get_samples_count()))
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
        std::cerr << "\ndiagnostic information:\n";
        std::cerr << boost::diagnostic_information(e);
    }
    std::cout << "finish\n";

    char ch;
    std::cin >> ch;
    return 0;
}


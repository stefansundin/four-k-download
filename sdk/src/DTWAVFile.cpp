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

#include <openmedia/DTWAVFile.h>

#include <cstdio>
#include <boost/filesystem/fstream.hpp>

#include <openmedia/DTError.h>
#include <openmedia/DTAudioFormat.h>
#include <openmedia/DTPacket.h>

#include "DTMediaMuxerImpl.h"
#include "DTUtils.h"

namespace openmedia {
namespace audio {

void Write16BitsLowHigh(FILE * fp, int i)
{
    putc(i & 0xff, fp);
    putc((i >> 8) & 0xff, fp);
} 

void Write32BitsLowHigh(FILE * fp, int i)
{
    Write16BitsLowHigh(fp, (int) (i & 0xffffL));
    Write16BitsLowHigh(fp, (int) ((i >> 16) & 0xffffL));
}

int write_wave_header(FILE * const fp, const int pcmbytes, const int freq, const int channels, const int bits)
{
    int     bytes = (bits + 7) / 8;

    /* quick and dirty, but documented */
    fwrite("RIFF", 1, 4, fp); /* label */
    Write32BitsLowHigh(fp, pcmbytes + 44 - 8); /* length in bytes without header */
    fwrite("WAVEfmt ", 2, 4, fp); /* 2 labels */
    Write32BitsLowHigh(fp, 2 + 2 + 4 + 4 + 2 + 2); /* length of PCM format declaration area */
    Write16BitsLowHigh(fp, 1); /* is PCM? */
    Write16BitsLowHigh(fp, channels); /* number of channels */
    Write32BitsLowHigh(fp, freq); /* sample frequency in [Hz] */
    Write32BitsLowHigh(fp, freq * channels * bytes); /* bytes per second */
    Write16BitsLowHigh(fp, channels * bytes); /* bytes per sample time */
    Write16BitsLowHigh(fp, bits); /* bits per sample */
    fwrite("data", 1, 4, fp); /* label */
    Write32BitsLowHigh(fp, pcmbytes); /* length in bytes of raw PCM data */

    return ferror(fp) ? -1 : 0;        
}

class wave_file_impl
{
public:
    wave_file_impl();
    ~wave_file_impl();
    void create(const char * _FileName, const int _SampleRate, const int _ChannelsCount, const int _BitPerSample); 
    void write_buffer(const boost::uint8_t * _Buffer, const size_t _SizeBytes);
    void close();

private:
    FILE * m_File;
    size_t m_SizeBytes;
    int m_SampleRate;
    int m_ChannelsCount;
    int m_BitPerSample;
};

wave_file_impl::wave_file_impl() : m_File(NULL),
    m_SizeBytes(0), m_SampleRate(44100),
    m_ChannelsCount(2), m_BitPerSample(16)
{}

wave_file_impl::~wave_file_impl()
{
    close();
}

void wave_file_impl::create(const char * _FileName, const int _SampleRate, const int _ChannelsCount, const int _BitPerSample)
{
    FILE * file = NULL;
    errno_t errNo = utils::dt_fopen(&file, _FileName, "wb");
    if (errNo || !file)
        BOOST_THROW_EXCEPTION(errors::error());
         
    write_wave_header(file, 0x7FFFFFFF, _SampleRate, _ChannelsCount, _BitPerSample);
    m_SampleRate    = _SampleRate;
    m_ChannelsCount = _ChannelsCount;
    m_BitPerSample  = _BitPerSample;
    m_File          = file;
}

void wave_file_impl::write_buffer(const boost::uint8_t * _Buffer, const size_t _SizeBytes)
{
    if (m_File)
    {
        size_t sizeWrite = 0;
        if (_SizeBytes)
            sizeWrite = fwrite(_Buffer, 1, _SizeBytes, m_File);
        m_SizeBytes += sizeWrite;
        DT_ASSERT(_SizeBytes == sizeWrite);
    }
    else
        throw std::logic_error("");
}

void wave_file_impl::close()
{
    if (m_File)
    {
        if (m_SizeBytes > 0xFFFFFFD0) 
        {
            m_SizeBytes = 0xFFFFFFD0; // TODO ?
        }

        if (!fseek(m_File, 0l, SEEK_SET))
            write_wave_header(m_File, m_SizeBytes, m_SampleRate, m_ChannelsCount, m_BitPerSample);

        fclose(m_File);
        m_File = NULL;
    }
}


} // namespace audio

/// \class  media_muxer_wavfile_impl
class media_muxer_wavfile_impl : public media_muxer::Impl
{
public:
    media_muxer_wavfile_impl(
        const char * _FileName,
        const audio_format * _AudioFormat
        );

public:
    virtual void open() {}
    virtual void write_packet(media_packet_ptr _MediaPacket);
    virtual void close();

private:
    audio::wave_file_impl impl_;

};

media_muxer_wavfile_impl::media_muxer_wavfile_impl(
        const char * _FileName,
        const audio_format * _AudioFormat
        )
{
    impl_.create(_FileName, _AudioFormat->get_sample_rate(), _AudioFormat->get_channels_count(), _AudioFormat->get_bits_per_sample());
}

void media_muxer_wavfile_impl::write_packet(media_packet_ptr _MediaPacket)
{
    if (_MediaPacket)
        impl_.write_buffer(_MediaPacket->get_data(), _MediaPacket->get_size());
}

void media_muxer_wavfile_impl::close()
{
    impl_.close();
}

media_muxer_wavfile::media_muxer_wavfile(
        const char * _FileName,
        const audio_format * _AudioFormat
        ) : media_muxer( new media_muxer_wavfile_impl(_FileName, _AudioFormat) )
{
    
}

} 

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

/// \file   DTMP3File.cpp

#include <openmedia/DTMP3File.h>
#include <openmedia/DTString.h>
#include "DTMediaMuxerImpl.h"
#include "DTFOpen.h"

#include <openmedia/DTError.h>
#include <openmedia/DTCodecExtraData.h>

#include <boost/filesystem/fstream.hpp>
#include <iostream>

#include "DTUtils.h"

namespace openmedia {

class media_muxer_mp3file_impl : public media_muxer::Impl
{
public:
    media_muxer_mp3file_impl(
        const char * _FileName,
        const audio_format * _AudioFormat,
        const codec_extra_data_ptr _CodecExtraData
        );

public:
    virtual void open() {};
    virtual void write_packet(media_packet_ptr _MediaPacket);
    virtual void close();

private:

    template <typename Char>
    void open_impl(Char * _FileName,
    const audio_format * _AudioFormat,
    codec_extra_data_ptr _CodecExtraData);

    void create_file(const char * _FileName);
    void write_to_mp3(void * buf, size_t Size);

private:

    codec_extra_data_ptr m_CodecExtraData;
    size_t m_id3v2_EndPos;

#if !(defined(_WIN32) || defined(__WIN32__) || defined(WIN32))
    fpos_t m_id3v2_EndPos_fpos;
#endif

private:
    boost::shared_ptr<FILE> outputFile_;
    
};

void media_muxer_mp3file_impl::write_to_mp3(void * buf, size_t Size)
{
    fwrite((const void*)buf, Size, 1, outputFile_.get());
}

media_muxer_mp3file::media_muxer_mp3file(
    const char * _FileName,
    const audio_format * _AudioFormat,
    codec_extra_data_ptr _CodecExtraData
    ) : media_muxer( new media_muxer_mp3file_impl(_FileName, _AudioFormat, _CodecExtraData) )
{
}

template <typename Char>
void media_muxer_mp3file_impl::open_impl(Char * _FileName,
               const audio_format * /*_AudioFormat*/,
               codec_extra_data_ptr _CodecExtraData)
{
    size_t dataIndex = 0;
    bool bRes = _CodecExtraData->get_index_by_name("ID3V2TAG", dataIndex);
    if (!bRes)
    {
        BOOST_THROW_EXCEPTION( errors::invalid_argument() );
    }

    const uint8_t * id3v2tag = _CodecExtraData->get_data(dataIndex);
    const size_t id3v2tag_size = _CodecExtraData->get_data_size(dataIndex);
    m_id3v2_EndPos = id3v2tag_size;

    create_file(_FileName);

    write_to_mp3((void*)id3v2tag, id3v2tag_size); // Windows Media Player troubles

#if !(defined(_WIN32) || defined(__WIN32__) || defined(WIN32))
    fgetpos(outputFile_.get(), &m_id3v2_EndPos_fpos);
#endif

    m_CodecExtraData = _CodecExtraData;

}

void media_muxer_mp3file_impl::create_file(const char * _FileName)
{

    FILEPtr f = dt_fopen(_FileName, "wb");

    if (!f)
    {
        BOOST_THROW_EXCEPTION( errors::create_file() );
        return;
    }

    outputFile_ = f;
}



media_muxer_mp3file_impl::media_muxer_mp3file_impl(
    const char * _FileName,
    const audio_format * _AudioFormat,
    codec_extra_data_ptr _CodecExtraData
    ) : m_id3v2_EndPos(0)
{
    open_impl(_FileName, _AudioFormat, _CodecExtraData);
}

void media_muxer_mp3file_impl::write_packet(media_packet_ptr _MediaPacket)
{
    if (!_MediaPacket)
        return;

    if (!outputFile_.get())
    {
        BOOST_THROW_EXCEPTION( errors::invalid_operation() );
        return;
    }

    const uint8_t * data = _MediaPacket->get_data();
    dt_data_size_t dataSize = _MediaPacket->get_size();
    DT_ASSERT(dataSize >= 0);

    if (dataSize > 0)
    {
        write_to_mp3((void*)data, dataSize);
    }
}

void media_muxer_mp3file_impl::close()
{
    size_t dataIndex = 0;
    bool bRes = m_CodecExtraData->get_index_by_name("ID3V1TAG", dataIndex);
    if (bRes)
    {
        uint8_t * data = m_CodecExtraData->get_data(dataIndex);
        size_t dataSize = m_CodecExtraData->get_data_size(dataIndex);

        write_to_mp3(data, dataSize);
    }

#if (defined(_WIN32) || defined(__WIN32__) || defined(WIN32))
    fpos_t pos = static_cast<fpos_t>(m_id3v2_EndPos);
    if (!fsetpos(outputFile_.get(), &pos)
        && m_CodecExtraData->get_index_by_name("LAMETAG", dataIndex)
        )
#else
    if (!fsetpos(outputFile_.get(), &m_id3v2_EndPos_fpos)
        && m_CodecExtraData->get_index_by_name("LAMETAG", dataIndex)
        )
#endif
    {
        uint8_t * data = m_CodecExtraData->get_data(dataIndex);
        size_t dataSize = m_CodecExtraData->get_data_size(dataIndex);    
        write_to_mp3(data, dataSize);
    }
    outputFile_.reset();
}


} // namespace openmedia

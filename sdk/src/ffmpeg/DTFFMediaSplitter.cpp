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

/// \file   DTAVMediaSplitter.cpp

#include <string>
#include <iostream>
#include <vector>

#define DT_FF_NEED_AVFORMAT
#include "DTFFHeader.h"

#include <openmedia/DTCommon.h>
#include <openmedia/DTError.h>
#include <openmedia/DTDecoderInfo.h>
#include "DTFFMediaSplitter.h"
#include "DTFFDecoderInfo.h"
#include <openmedia/DTString.h>

#include "../DTUtils.h"
#include "../DTMediaSplitterImpl.h"
#include "../DTDecoderInfoImpl.h"

#include "DTFFPacket.h"
//#include "DTFFMetadata.h"
#include "DTFFStreamInfo.h"
#include "DTFFFileInfo.h"

namespace openmedia { namespace details {

class _AVPacketDestruct
{
public:
    void operator()(AVPacket * _Packet)
    {
        if (_Packet)
        {
            av_free_packet(_Packet);
            av_free(_Packet);
        }
    }
};

class _AVFormatContextDestruct
{
public:
    void operator()(AVFormatContext * _AVFormatContext)
    {
        if (_AVFormatContext)
        {
            av_close_input_file(_AVFormatContext);
        }
    }
};

AVFormatContextPtr dt_av_open_input_file(const char * _Filename,
                                         AVInputFormat * _InputFormat,
                                         int _BufferSize,
                                         AVFormatParameters * _FormatParameters)
{
    AVFormatContext * inputFormatContext = NULL;
    int dt_err = av_open_input_file(&inputFormatContext, _Filename, _InputFormat, _BufferSize, _FormatParameters);
    FF_CHECK_ERR(dt_err);
    if (FF_ERR(dt_err))
    {
        DT_THROW_AVERROR(media_splitter::error(), dt_err, "av_open_input_file");
        return AVFormatContextPtr((AVFormatContext*)NULL);
    }

    return AVFormatContextPtr(inputFormatContext, _AVFormatContextDestruct());
}

void dt_av_find_stream_info(AVFormatContext * _AVFormatContext)
{
    int dt_err = av_find_stream_info(_AVFormatContext);
    FF_CHECK_ERR(dt_err);

    if (FF_ERR(dt_err))
    {
        DT_THROW_AVERROR(media_splitter::error(), dt_err, "av_find_stream_info");
        return;
    }
}

AVPacketPtr dt_av_create_packet()
{
    AVPacket * packet = (AVPacket *)::av_malloc(sizeof(AVPacket));
    if (NULL == packet)
    {
        BOOST_THROW_EXCEPTION(errors::bad_alloc());        
    }
    
    av_init_packet(packet);

    return AVPacketPtr(packet, details::_AVPacketDestruct());
}

AVPacketPtr dt_av_read_frame(AVFormatContext * _FormatContext, bool & _Eof)
{
    DT_ASSERT(NULL != _FormatContext);
    if (NULL == _FormatContext)
        BOOST_THROW_EXCEPTION(errors::invalid_operation());
    
    _Eof = false;
    
    AVPacketPtr packet = dt_av_create_packet();;

    int dt_err = AVERROR(EAGAIN);
    while (AVERROR(EAGAIN) == dt_err) 
        dt_err = av_read_frame(_FormatContext, packet.get());

    if (AVERROR_EOF != dt_err)
    {
        FF_CHECK_ERR(dt_err);
    }

    if (dt_err >= 0)
    {
        dt_err = av_dup_packet(packet.get());
        FF_CHECK_ERR(dt_err);
        return packet;
    } 
    else
    {
        if (AVERROR_EOF != dt_err && AVERROR(EIO) != dt_err)
        {
            DT_THROW_AVERROR(media_splitter::error(), dt_err, "av_read_frame");
            return packet;
        } 
        else
        {
            _Eof = true;
            return AVPacketPtr((AVPacket*)NULL);        
        }
    }
}

void dt_av_seek_frame(AVFormatContext * _FormatContext, int _StreamIndex, dt_ts_t _Pos, int _Flags)
{
    DT_ASSERT(NULL != _FormatContext);
    if (NULL == _FormatContext)
        BOOST_THROW_EXCEPTION(errors::invalid_operation());

    int dt_err = av_seek_frame(_FormatContext, _StreamIndex, _Pos, _Flags);
    FF_CHECK_ERR(dt_err);
	
    if (FF_ERR(dt_err))
    {
        DT_THROW_AVERROR(media_splitter::error(), dt_err, "av_seek_frame");
    }
}

} // namespace details


/// \class  av_media_splitter_impl
class av_media_splitter_impl: public media_splitter::Impl
{
public:
    av_media_splitter_impl(const char * _Filename);
#if defined(DT_CONFIG_HAVE_UTF16_OPEN) && (1 == DT_CONFIG_HAVE_UTF16_OPEN)
    av_media_splitter_impl(const wchar_t * _Filename);
#endif
    av_media_splitter_impl(AVFormatContextPtr _AVFormatcontext);
    virtual ~av_media_splitter_impl();

public:
    virtual media_packet_ptr            read_packet();
    virtual void                        seek(dt_ts_t _Position);
    virtual file_info_general_ptr       get_file_info() const;
    virtual stream_info_ptr             get_stream_info(unsigned int _StreamIndex) const;
    virtual bool                        is_eof() const;

private:
    static void set_streams_type_and_base(std::vector<dt_media_type_t> & _TypeVector,
        std::vector<dt_rational_t> & _TimeBaseVector,
        const AVFormatContext * _FormatContext);

    void av_media_splitter_impl_do(const char * Filename);
    
private:
    AVFormatContextPtr      m_FormatContextPtr;
    file_info_general_ptr   m_FileInfoGeneral;
    bool                    m_Eof;
    std::vector<dt_media_type_t> m_MediaTypes;
    std::vector<dt_rational_t> m_TimeBases;
};

// av_media_splitter_impl impl

void av_media_splitter_impl::set_streams_type_and_base(std::vector<dt_media_type_t> & _TypeVector,
        std::vector<dt_rational_t> & _TimeBaseVector,
        const AVFormatContext * _FormatContext)
{
    _TypeVector.resize(0);
    _TimeBaseVector.resize(0);
    if (!_FormatContext)
        return;

    const unsigned int streamsCount = _FormatContext->nb_streams;
    _TypeVector.resize( streamsCount );
    _TimeBaseVector.resize(  streamsCount );
    for (size_t s = 0; s < streamsCount; ++s )
    {
        if (AVStream * stream = _FormatContext->streams[s])
        {
            _TimeBaseVector[s] = FF2DTType( stream->time_base );
            if (AVCodecContext * codec = stream->codec)
            {
                _TypeVector[s] = FF2DTType( codec->codec_type );
            }
        }                
    }
}

av_media_splitter_impl::av_media_splitter_impl(const char * _Filename) : m_Eof(true)
{
    std::string fileName;
#if defined(DT_CONFIG_HAVE_UTF16_OPEN) && (1 == DT_CONFIG_HAVE_UTF16_OPEN)
    std::string ansiName = _Filename;
    fileName = ansi_to_utf8(ansiName);
#else
    fileName = _Filename;
#endif
    av_media_splitter_impl_do( fileName.c_str() );
}

#if defined(DT_CONFIG_HAVE_UTF16_OPEN) && (1 == DT_CONFIG_HAVE_UTF16_OPEN)
av_media_splitter_impl::av_media_splitter_impl(const wchar_t * _Filename) : m_Eof(true)
{
    std::wstring unt16Name = _Filename;
    std::string fileName = utf16_to_utf8(unt16Name);
    av_media_splitter_impl_do( fileName.c_str() );
}
#endif

void av_media_splitter_impl::av_media_splitter_impl_do(const char * Filename)
{
    AVFormatContextPtr formatContextPtr = details::dt_av_open_input_file(Filename, NULL, 0, NULL);
    details::dt_av_find_stream_info(formatContextPtr.get());
    m_FileInfoGeneral = file_info_general_ptr( new ff_file_info_general(formatContextPtr.get()) );
    
    set_streams_type_and_base(m_MediaTypes, m_TimeBases, formatContextPtr.get());
    m_FormatContextPtr = formatContextPtr;
    m_Eof = false;
}

av_media_splitter_impl::av_media_splitter_impl(AVFormatContextPtr _AVFormatcontext)
{
    m_FileInfoGeneral = file_info_general_ptr( new ff_file_info_general(_AVFormatcontext.get()) );
    set_streams_type_and_base(m_MediaTypes, m_TimeBases, _AVFormatcontext.get());
    m_FormatContextPtr = _AVFormatcontext;
}

file_info_general_ptr av_media_splitter_impl::get_file_info() const
{
    return m_FileInfoGeneral;    
}

media_packet_ptr av_media_splitter_impl::read_packet()
{
    bool eofVal = false;
    AVPacketPtr avPacketPtr = details::dt_av_read_frame(m_FormatContextPtr.get(), eofVal);
    m_Eof = eofVal;
    if (!eofVal)
    {
        dt_media_type_t mediaType = DT_AVMEDIA_TYPE_UNKNOWN;
        dt_rational_t timeBase = dt_rational_t();
        if (avPacketPtr.get())
        {
            mediaType = m_MediaTypes[ avPacketPtr->stream_index ];
            timeBase = m_TimeBases[ avPacketPtr->stream_index ];
        }
        return media_packet_ptr(new ff_media_packet(avPacketPtr, mediaType, timeBase));
    }
    else
    {
        return media_packet_ptr((media_packet*)NULL);
    }
};

void av_media_splitter_impl::seek(dt_ts_t _Position)
{
    details::dt_av_seek_frame(m_FormatContextPtr.get(), -1, _Position, 0);
};

stream_info_ptr av_media_splitter_impl::get_stream_info(unsigned int _StreamIndex) const
{
    if (!m_FormatContextPtr)
    {
        DT_ASSERT(false);
        BOOST_THROW_EXCEPTION(errors::invalid_operation());
        return stream_info_ptr((stream_info *)NULL);
    }

    if (_StreamIndex >= m_FormatContextPtr->nb_streams)
    {
        DT_ASSERT(false);
        BOOST_THROW_EXCEPTION(errors::invalid_argument());
        return stream_info_ptr((stream_info *)NULL);
    }

    return ff_stream_info::create(m_FormatContextPtr->streams[_StreamIndex]);
}

bool av_media_splitter_impl::is_eof() const
{
    return m_Eof;
}

av_media_splitter_impl::~av_media_splitter_impl()
{
}

// ff_media_splitter impl

ff_media_splitter::ff_media_splitter(const char * _Filename) :
        media_splitter(new av_media_splitter_impl(_Filename))
{
}

#if defined(DT_CONFIG_HAVE_UTF16_OPEN) && (1 == DT_CONFIG_HAVE_UTF16_OPEN)        
ff_media_splitter::ff_media_splitter(const wchar_t * _Filename) :
        media_splitter(new av_media_splitter_impl(_Filename))
{
}
#endif

ff_media_splitter::ff_media_splitter(AVFormatContextPtr _Formatcontext) :
        media_splitter(new av_media_splitter_impl(_Formatcontext))
{
}

} // namespace openmedia

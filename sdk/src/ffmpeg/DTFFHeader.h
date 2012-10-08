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


/// \file   DTFFHeader.h

#ifndef _DT_FFHEADERS_COMMON_H_
#define _DT_FFHEADERS_COMMON_H_

#include <boost/static_assert.hpp>
#include <boost/cstdint.hpp>
#include <openmedia/DTError.h>
#include <openmedia/DTTypes.h>
#include <openmedia/DTMediaTypes.h>
#include <openmedia/DTCodecTypes.h>
#include <openmedia/DTPixFormatTypes.h>
#include <openmedia/DTSampleFormatTypes.h>

////////////////////////////////////////////////////////////////////////////////////////////////
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4244)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include <libswscale/swscale.h>
#include <libavcodec/avcodec.h>
#include <libswresample/swresample.h>

#ifdef __cplusplus
}
#endif

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4244)
#endif

#ifdef __cplusplus
extern "C" {
#endif
#include <libavutil/opt.h>
#include <libavutil/avutil.h>
#include <libavutil/mem.h>
#include <libavutil/mathematics.h>
#include <libavutil/rational.h>
#ifdef __cplusplus
}
#endif

#if defined(_MSC_VER)
#pragma warning(pop)
#endif


////////////////////////////////////////////////////////////////////////////////////////////////

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4244)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include <libavformat/avformat.h>
#include <libavformat/avio.h>
#include <libavformat/url.h>

#ifdef __cplusplus
}
#endif

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

////////////////////////////////////////////////////////////////////////////////////////////////

inline openmedia::dt_rational_t FF2DTType(const AVRational & _AVRational)
{
    return openmedia::dt_rational_t(_AVRational.num, _AVRational.den);
}

inline AVRational DT2FFType(const openmedia::dt_rational_t & _DTRational)
{
    AVRational rational;
    rational.num = _DTRational.num;
    rational.den = _DTRational.den;
    return rational;
}

inline openmedia::dt_media_type_t FF2DTType(const AVMediaType _AVMediaType)
{
    return static_cast<openmedia::dt_media_type_t>(_AVMediaType);
}

inline AVMediaType DT2FFType(const openmedia::dt_media_type_t _DTMediaType)
{
    return static_cast<AVMediaType>(_DTMediaType);
}

inline openmedia::dt_pixel_format_t FF2DTType(const PixelFormat _AVPixelFormat)
{
    return static_cast<openmedia::dt_pixel_format_t>(_AVPixelFormat);
}

inline PixelFormat DT2FFType(const openmedia::dt_pixel_format_t _DTPixelFormat)
{
    return static_cast<PixelFormat>(_DTPixelFormat);
}

inline AVSampleFormat DT2FFType(const openmedia::dt_sample_format_t _DTSampleFormat)
{
    return static_cast<AVSampleFormat>(_DTSampleFormat);
}

inline openmedia::dt_sample_format_t FF2DTType(const AVSampleFormat _AVSampleFormat)
{
    return static_cast<openmedia::dt_sample_format_t>(_AVSampleFormat);
}

inline CodecID DT2FFType(const openmedia::dt_codec_id_t _DTCodecID)
{
    return static_cast<CodecID>(_DTCodecID);
}

inline openmedia::dt_codec_id_t FF2DTType(const CodecID _FFCodecID)
{
    return static_cast<openmedia::dt_codec_id_t>(_FFCodecID);
}

#define FF_ERR(ERR)     ((ERR) < 0)
#define FF_NOERR(ERR)   ((ERR) >= 0)

#define DT_THROW_AVERROR(EX, ERR, FUNCNAME) BOOST_THROW_EXCEPTION( (EX)<< boost::errinfo_errno(AVUNERROR(ERR)) << boost::errinfo_api_function(FUNCNAME) )

#define FF_CHECK_ERR(ERR)

class InitializeFFMPEG
{
public:
    static bool initialized;
    InitializeFFMPEG();
};

namespace 
{
    InitializeFFMPEG initializeFFMPEG;
    //const bool ffmpegInit = InitializeFFMPEG::initialized;
}

#endif // #ifndef _DT_FFHEADERS_COMMON_H_

#if 0

#ifndef _DTFFHHEADER_H_INCLUDED_
#define _DTFFHHEADER_H_INCLUDED_

//#define FF_API_OLD_SAMPLE_FMT (1 == 1)

#include <boost/cstdint.hpp>

#ifdef __cplusplus
extern "C" {
#endif
#include "libavformat/avformat.h"
//#include "libavcore/audioconvert.h"
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
#include "libavutil/avutil.h"
#include "libavformat/metadata.h"
#include "libavcodec/audioconvert.h"
#include "libavutil/pixdesc.h"
#ifdef __cplusplus
}
#endif



#include <boost/static_assert.hpp>

#include <openmedia/DTCommon.h>
#include <openmedia/DTTypes.h>
#include <openmedia/DTError.h>

#include <openmedia/DTPixFormatTypes.h>
#include <openmedia/DTSampleFormatTypes.h>
#include <openmedia/DTMediaTypes.h>
#include <openmedia/DTCodecTypes.h>

class InitializeFFMPEG
{
public:
    InitializeFFMPEG();

private:
    static bool bInitialized;    
};

namespace {
InitializeFFMPEG initializeFFMPEG;
}

BOOST_STATIC_ASSERT(sizeof(openmedia::dt_rational_t) == sizeof(AVRational));
BOOST_STATIC_ASSERT((int)openmedia::DT_AVMEDIA_TYPE_NB == (int)AVMEDIA_TYPE_NB);
BOOST_STATIC_ASSERT((int)openmedia::DT_PIX_FMT_NB == (int)PIX_FMT_NB);
BOOST_STATIC_ASSERT((int)openmedia::DT_SAMPLE_FMT_NB == (int)SAMPLE_FMT_NB);
//BOOST_STATIC_ASSERT((size_t)openmedia::DT_FF_INPUT_BUFFER_PADDING_SIZE == (size_t)FF_INPUT_BUFFER_PADDING_SIZE);

#if (!defined(LIBAVCODEC_VERSION_MAJOR)) || (LIBAVCODEC_VERSION_MAJOR >= 53)
BOOST_STATIC_ASSERT(false);
#endif

inline openmedia::dt_rational_t FF2DTType(const AVRational & _AVRational)
{
    return openmedia::dt_rational_t(_AVRational.num, _AVRational.den);
}

inline AVRational DT2FFType(const openmedia::dt_rational_t & _DTRational)
{
    AVRational rational;
    rational.num = _DTRational.num;
    rational.den = _DTRational.den;
    return rational;
}

inline openmedia::dt_media_type_t FF2DTType(const AVMediaType _AVMediaType)
{
    return static_cast<openmedia::dt_media_type_t>(_AVMediaType);
}

inline AVMediaType DT2FFType(const openmedia::dt_media_type_t _DTMediaType)
{
    return static_cast<AVMediaType>(_DTMediaType);
}

inline openmedia::dt_pixel_format_t FF2DTType(const PixelFormat _AVPixelFormat)
{
    return static_cast<openmedia::dt_pixel_format_t>(_AVPixelFormat);
}

inline PixelFormat DT2FFType(const openmedia::dt_pixel_format_t _DTPixelFormat)
{
    return static_cast<PixelFormat>(_DTPixelFormat);
}

inline openmedia::dt_sample_format_t FF2DTType(const SampleFormat _AVSampleFormat)
{
    return static_cast<openmedia::dt_sample_format_t>(_AVSampleFormat);
}

inline SampleFormat DT2FFType(const openmedia::dt_sample_format_t _DTSampleFormat)
{
    return static_cast<SampleFormat>(_DTSampleFormat);
}

inline CodecID DT2FFType(const openmedia::dt_codec_id_t _DTCodecID)
{
    return static_cast<CodecID>(_DTCodecID);
}

inline openmedia::dt_codec_id_t FF2DTType(const CodecID _FFCodecID)
{
    return static_cast<openmedia::dt_codec_id_t>(_FFCodecID);
}

#define FF_ERR(ERR)     ((ERR) < 0)
#define FF_NOERR(ERR)   ((ERR) >= 0)

#define DT_THROW_AVERROR(EX, ERR, FUNCNAME) BOOST_THROW_EXCEPTION( (EX)<< boost::errinfo_errno(AVUNERROR(ERR)) << boost::errinfo_api_function(FUNCNAME) )

#define FF_CHECK_ERR(ERR)

#endif // #ifndef _DTTYPES_H_INCLUDED_
#endif
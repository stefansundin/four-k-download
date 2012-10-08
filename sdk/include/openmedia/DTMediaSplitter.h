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


/// \file   DTMediaSplitter.h

#ifndef _DTMEDIASPLITTER_H_INCLUDED_
#define _DTMEDIASPLITTER_H_INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <openmedia/DTConfig.h>
#include <openmedia/DTDeclareImpl.h>
#include <openmedia/DTTypes.h>
#include <openmedia/DTCStdInt.h>
#include <openmedia/DTCommon.h>
#include <openmedia/DTError.h>

namespace openmedia {

class media_packet;
typedef boost::shared_ptr<media_packet> media_packet_ptr;

class file_info_general;
typedef boost::shared_ptr<file_info_general> file_info_general_ptr;

class stream_info;
typedef boost::shared_ptr<stream_info> stream_info_ptr;

class media_splitter;
typedef boost::shared_ptr<media_splitter> media_splitter_ptr;

/// \class  media_splitter
class _OPENMEDIASDK_API media_splitter : interface_base
{
    DT_DECLARE_IMPL2(media_splitter);
public:
    media_packet_ptr            read_packet();
    void                        seek(dt_ts_t _Position);
    file_info_general_ptr       get_file_info() const;
    virtual                     ~media_splitter();
    stream_info_ptr             get_stream_info(unsigned int _StreamIndex) const;
    bool                        is_eof();

public:
    struct error : virtual errors::dt_error {};

protected:
    media_splitter(media_splitter::Impl * _Impl);

private:
    media_splitter::Impl * m_pImpl;
};

/// \class  media_splitter_creator
class _OPENMEDIASDK_API media_splitter_creator
{
public:
    static media_splitter_ptr create(const char * _Filename);
};


} // namespace openmedia 


#endif

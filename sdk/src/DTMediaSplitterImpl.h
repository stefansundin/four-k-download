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


/// \file   DTMediaSplitterImpl.h

#ifndef _DTMEDIASPLITTERIMPL_H_INCLUDED_
#define _DTMEDIASPLITTERIMPL_H_INCLUDED_
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <openmedia/DTCommon.h>
#include <openmedia/DTMediaSplitter.h>

namespace openmedia {

/// \class  media_splitter::Impl
class media_splitter::Impl : public impl_base
{
public:
    virtual media_packet_ptr            read_packet() = 0;
    virtual void                        seek(dt_ts_t _Position) = 0;
    virtual file_info_general_ptr       get_file_info() const = 0;
    virtual stream_info_ptr             get_stream_info(unsigned int _StreamIndex) const = 0;
    virtual bool                        is_eof() const = 0;

};

} // namespace openmedia 

#endif

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

#include <openmedia/DTError.h>
#include <openmedia/DTString.h>
#include "DTRAWFile.h"
#include "DTMediaMuxerImpl.h"
#include "DTFOpen.h"

namespace openmedia {

class raw_file_impl : public media_muxer::Impl
{
public:
    raw_file_impl(FILEPtr f) : file_(f)
    {
        if (!f)
            BOOST_THROW_EXCEPTION( errors::create_file() );
    }

public:
    virtual void open() {}

    virtual void write_packet(media_packet_ptr _MediaPacket)
    {
        if (!file_)
            BOOST_THROW_EXCEPTION( errors::unexpected() );

        fwrite(_MediaPacket->get_data(), _MediaPacket->get_size(), 1, file_.get());
    }

    virtual void close()
    {
        file_.reset();
    }

private:
    FILEPtr file_;
    
};

raw_file::raw_file(const char * FileNameUtf8) 
: media_muxer( new raw_file_impl( dt_fopen(FileNameUtf8, "wb") ) )
{
}

raw_file::raw_file(const std::string& FileNameUtf8)
: media_muxer( new raw_file_impl( dt_fopen(FileNameUtf8, "wb") ) )
{
}

}

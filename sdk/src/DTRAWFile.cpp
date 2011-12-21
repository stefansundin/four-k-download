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

#include <openmedia/DTError.h>
#include <openmedia/DTString.h>
#include "DTRAWFile.h"
#include "DTMediaMuxerImpl.h"

namespace openmedia {

class raw_file_impl : public media_muxer::Impl
{
public:
    raw_file_impl(FILE * f) : file_(f, &fclose)
    {
        if (!f)
            BOOST_THROW_EXCEPTION( errors::create_file() );
    }

public:
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
    boost::shared_ptr<FILE> file_;
    
};

raw_file::raw_file(const char * FileName) 
: media_muxer( new raw_file_impl( fopen(FileName, "wb") ) )
{
}

#if defined(DT_CONFIG_HAVE_UTF16_OPEN) && (1 == DT_CONFIG_HAVE_UTF16_OPEN)
raw_file::raw_file(const wchar_t * FileName) 
: media_muxer( new raw_file_impl( _wfopen(FileName, L"wb") ) )
{
}
#else
raw_file::raw_file(const wchar_t * FileName) 
: media_muxer( new raw_file_impl( fopen( utf16_to_utf8( ::std::wstring(FileName) ).c_str(), "wb") ) )
{
}
#endif

}

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


/// \file   DTDecoder.h

#ifndef _DTDECODER_H_INCLUDED_
#define _DTDECODER_H_INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <openmedia/DTConfig.h>
#include <openmedia/DTDeclareImpl.h>
#include <openmedia/DTCommon.h>
#include <openmedia/DTError.h>

namespace openmedia {

class decoder;
typedef boost::shared_ptr<decoder> decoder_ptr;

///  \class decoder
class _OPENMEDIASDK_API decoder: interface_base
{
    DT_DECLARE_IMPL2(decoder);
public:
    void reset();
    virtual ~decoder() = 0;

public:
    struct create_error : public virtual errors::dt_error
    {
    public:
        create_error() {} ;    
    };

    struct decode_error: public virtual errors::dt_error
    {
    public:
        decode_error() {} ;    
    };

protected:
    decoder(decoder::Impl * _Impl);

private:
    decoder::Impl * m_pImpl;

};

template <class MediaData>
class MediaDecoderSelector
{
public:
    typedef void MediaDecoder;
};

} // namespace openmedia 

#endif

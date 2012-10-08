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

#include "DTFFHeader.h"

#include <openmedia/DTConfig.h>
#include <openmedia/DTAssert.h>
#include <boost/thread/mutex.hpp>
#include "DTFFUTF8File.h"

namespace  {
int dt_ff_lockmgr_cb(void **mutex, enum AVLockOp op)
{
    DT_ASSERT(NULL != mutex);
    if (NULL == mutex)
        return -1;

    switch(op)
    {
    case AV_LOCK_CREATE:
        {
            *mutex = NULL;
            boost::mutex * m = new boost::mutex();
            *mutex = static_cast<void*>(m);
            break;
        }
    case AV_LOCK_OBTAIN:
        {
            boost::mutex * m =  static_cast<boost::mutex*>(*mutex);
            DT_ASSERT(NULL != m);
            m->lock();
            break;
        }
    case AV_LOCK_RELEASE:
        {
            boost::mutex * m = static_cast<boost::mutex*>(*mutex);
            DT_ASSERT(NULL != m);
            m->unlock();
            break;
        }
        break;
    case AV_LOCK_DESTROY:
        {
            boost::mutex * m = static_cast<boost::mutex*>(*mutex);
            DT_ASSERT(NULL != m);
            delete m;
            break;
        }
        break;
    default:
        DT_ASSERT(false);
        break;
    }
    return 0;
}

bool register_ffmpeg_lock()
{
    return  ( av_lockmgr_register(&dt_ff_lockmgr_cb) >= 0 );        
}

}


bool InitializeFFMPEG::initialized = false;

InitializeFFMPEG::InitializeFFMPEG()
{
    if (!initialized)
    {
        av_register_all();
        avcodec_register_all();
        register_ffmpeg_lock();        
        #ifdef _WIN32
        #if defined(DT_CONFIG_HAVE_UTF16_OPEN) && (1 == DT_CONFIG_HAVE_UTF16_OPEN)
            setlocale(LC_ALL,"");
            openmedia::ReplaceFFMPEGFileProtocol();
        #endif
        #endif
        initialized = true;
    }
}

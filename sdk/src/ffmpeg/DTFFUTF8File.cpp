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


#ifdef _WIN32
#include "DTFFHeader.h"
#include <openmedia/DTCommon.h>
#include <openmedia/DTString.h>
#include <openmedia/DTPlatform.h>
#include "../DTUtils.h"

#include <string>
#include <sys/stat.h>
#include <io.h>
#include <fcntl.h>
#include <stdlib.h>

namespace openmedia {

int av_strstart_(const char *str, const char *pfx, const char **ptr)
{
    while (*pfx && *pfx == *str) {
        pfx++;
        str++;
    }
    if (!*pfx && ptr)
        *ptr = str;
    return !*pfx;
} 

static int wfile_open(URLContext *h, const char *filename, int flags)
{
    int access;
    int fd;

    av_strstart_(filename, "file:", &filename);
    std::string fileName = filename;
    std::wstring wfileName = utf8_to_utf16(fileName);

    /// FOR FFMPEG 0.7
#if 0
    if (flags & URL_RDWR) 
        access = O_CREAT | O_TRUNC | O_RDWR;
    else if (flags & URL_WRONLY) 
        access = O_CREAT | O_TRUNC | O_WRONLY;
    else 
        access = O_RDONLY;
#else
    /// FOR FFMPEG 0.8
    if (flags & AVIO_FLAG_WRITE && flags & AVIO_FLAG_READ) {
        access = O_CREAT | O_TRUNC | O_RDWR;
    } else if (flags & AVIO_FLAG_WRITE) {
        access = O_CREAT | O_TRUNC | O_WRONLY;
    } else {
        access = O_RDONLY;
    } 
#endif

#ifdef O_BINARY
    access |= O_BINARY;
#endif

    errno_t errNo = utils::dt_wopen(&fd, wfileName.c_str(), access, 0666);
    if (fd == -1 || errNo)
    {
        return AVERROR(errNo);
    }

    h->priv_data = (void *) (intptr_t) fd;
    return 0;
}

typedef int ( *file_read_f)(URLContext *, unsigned char *, int);

typedef  int64_t (*file_seek_f)(URLContext *, int64_t, int);

static file_read_f old_file_read;
static file_seek_f old_file_seek;

static int wfile_read(URLContext *h, unsigned char *buf, int size)
{
    int fd = (intptr_t) h->priv_data;
    int res = _read(fd, buf, size);
    return res;
}

static int wfile_write(URLContext *h, const unsigned char *buf, int size)
{
    int fd = (intptr_t) h->priv_data;
    return _write(fd, buf, size);
}

static int64_t wfile_seek(URLContext *h, int64_t pos, int whence)
{
#if defined(_MSC_VER)
    int fd = (intptr_t) h->priv_data;
    if (whence == AVSEEK_SIZE) 
    {
        struct _stat64 st;
        int ret = _fstat64(fd, &st);
        return ret < 0 ? AVERROR(errno) : st.st_size;
    }
    return _lseeki64(fd, pos, whence);
#else
    int fd = (intptr_t) h->priv_data;
    if (whence == AVSEEK_SIZE) 
    {
        struct stat st;
        int ret = fstat(fd, &st);
        return ret < 0 ? AVERROR(errno) : st.st_size;
    }
    return _lseeki64(fd, pos, whence);

#endif
}

static int wfile_get_handle(URLContext *h)
{
    return (intptr_t) h->priv_data;
}

static int wfile_close(URLContext *h)
{
    int fd = (intptr_t) h->priv_data;
    return _close(fd);
}

void ReplaceFFMPEGFileProtocol()
{
    URLProtocol * protocol = av_protocol_next(NULL);
    while (protocol && std::string(protocol->name) != "file")
        protocol = av_protocol_next(protocol);

    if (protocol)
    {
        protocol->url_open = &wfile_open;

        protocol->url_read = &wfile_read;
        protocol->url_seek = &wfile_seek;
        protocol->url_write = &wfile_write;
        protocol->url_get_file_handle = &wfile_get_handle;
        protocol->url_close = &wfile_close;
    }
};


}
#endif
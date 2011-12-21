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


#include "viewmodel/mediadetailsviewmodel.h"
#include "viewmodel/mediadownload.h"

using namespace ViewModel;
using namespace ComponentModel;
using namespace Mvvm;
using namespace openmedia;


MediaDetailsViewModel::MediaDetailsViewModel(const Mvvm::Dialog* dialog, MediaDownloadType downloadType,
                      openmedia::downloader::media_download_list_ptr mediaList, int mediaIndex,
                      QObject *parent) :
    NotifyObject(parent),
    m_dialog(dialog),
    m_downloadType(downloadType)
{
    m_mediaQuality = mediaList->media(mediaIndex).quality();
    m_mediaContent = mediaList->media(mediaIndex).content_type();
    m_summary = MediaDownloadDetails::summary(downloadType, mediaList, mediaIndex),
    m_quality = MediaDownloadDetails::quality(downloadType, mediaList, mediaIndex);
    m_codecs = MediaDownloadDetails::codecs(downloadType, mediaList, mediaIndex);
    m_format = MediaDownloadDetails::format(downloadType, mediaList, mediaIndex);
    m_fileExt = MediaDownloadDetails::fileExt(downloadType, mediaList, mediaIndex);
    m_fileSize = MediaDownloadDetails::fileSize(downloadType, mediaList, mediaIndex);
}


MediaDetailsViewModel::MediaDetailsViewModel(const Mvvm::Dialog* dialog, MediaDownloadType downloadType,
                      openmedia::downloader::media_quality_type_t mediaQuality,
                      openmedia::downloader::media_content_type_t mediaContent,
                      const QString& summary, const QString& quality, const QString& codecs,
                      const QString& format, const QString& fileExt, const QString& fileSize,
                      QObject *parent) :
    NotifyObject(parent),
    m_dialog(dialog),
    m_downloadType(downloadType),
    m_mediaQuality(mediaQuality),
    m_mediaContent(mediaContent),
    m_summary(summary),
    m_quality(quality),
    m_codecs(codecs),
    m_format(format),
    m_fileExt(fileExt),
    m_fileSize(fileSize)
{
}


MediaDownloadType MediaDetailsViewModel::downloadType() const
{
    return m_downloadType;
}


openmedia::downloader::media_quality_type_t MediaDetailsViewModel::mediaQuality() const
{
    return m_mediaQuality;
}


openmedia::downloader::media_content_type_t MediaDetailsViewModel::mediaContent() const
{
    return m_mediaContent;
}


QString MediaDetailsViewModel::summary() const
{
    return m_summary;
}


QString MediaDetailsViewModel::quality() const
{
    return m_quality;
}


QString MediaDetailsViewModel::codecs() const
{
    return m_codecs;
}


QString MediaDetailsViewModel::format() const
{
    return m_format;
}


QString MediaDetailsViewModel::fileExt() const
{
    return m_fileExt;
}


QString MediaDetailsViewModel::fileSize() const
{
    return m_fileSize;
}

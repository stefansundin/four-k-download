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


#ifndef MEDIADOWNLOAD_H
#define MEDIADOWNLOAD_H

#include <QObject>
#include <QMetaType>
#include "openmedia/DTMediaDownloader.h"


namespace ViewModel
{

typedef enum
{
    DownloadVideo,
    ExtractAudio

} MediaDownloadType;


class MediaDownloadDetails
{
public:
    static QString summary(MediaDownloadType downloadType, openmedia::downloader::media_quality_type_t quality);
    static QString quality(MediaDownloadType downloadType, openmedia::downloader::media_quality_type_t quality);
    static QString format(MediaDownloadType downloadType, openmedia::downloader::media_content_type_t format);
    static QString codecs(MediaDownloadType downloadType, openmedia::downloader::media_content_type_t format, openmedia::downloader::media_video_type_t video, openmedia::downloader::media_audio_type_t audio);
    static QString fileExt(MediaDownloadType downloadType, openmedia::downloader::media_content_type_t format);

    static QString summary(MediaDownloadType downloadType, const openmedia::downloader::media_download_list_ptr mediaList, int mediaIndex);
    static QString quality(MediaDownloadType downloadType, const openmedia::downloader::media_download_list_ptr mediaList, int mediaIndex);
    static QString format(MediaDownloadType downloadType, const openmedia::downloader::media_download_list_ptr mediaList, int mediaIndex);
    static QString codecs(MediaDownloadType downloadType, const openmedia::downloader::media_download_list_ptr mediaList, int mediaIndex);
    static QString fileExt(MediaDownloadType downloadType, const openmedia::downloader::media_download_list_ptr mediaList, int mediaIndex);
    static QString fileSize(MediaDownloadType downloadType, const openmedia::downloader::media_download_list_ptr mediaList, int mediaIndex);
};


class MediaDownloadItem
{
public:
    MediaDownloadItem();
    MediaDownloadItem(MediaDownloadType downloadType, openmedia::downloader::media_download_list_ptr mediaList, int mediaIndex,
                      int subtitlesIndex, QString outputPath, bool needGenerate = false);

    MediaDownloadType downloadType() const;
    openmedia::downloader::media_download_list_ptr mediaList() const;
    openmedia::downloader::media_download_info media() const;
    int mediaIndex() const;
    int subtitlesIndex() const;
    QString mediaFileName() const;
    QString subtitlesFileName() const;

private:
    MediaDownloadType m_downloadType;
    openmedia::downloader::media_download_list_ptr m_mediaList;
    int m_mediaIndex;
    int m_subtitlesIndex;
    QString m_outputPath;
    bool m_needGenerate;
};


typedef QList<MediaDownloadItem> MediaDownloadList;

} // ViewModel

Q_DECLARE_METATYPE(ViewModel::MediaDownloadType)
Q_DECLARE_METATYPE(ViewModel::MediaDownloadItem)
Q_DECLARE_METATYPE(ViewModel::MediaDownloadList)

#endif // MEDIADOWNLOAD_H

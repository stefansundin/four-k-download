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


#include "viewmodel/mediadownload.h"
#include "componentmodel/transform.h"
#include "componentmodel/filesystem.h"
#include <QFileInfo>
#include <QDir>

using namespace ViewModel;
using namespace ComponentModel;
using namespace openmedia;


QString MediaDownloadDetails::summary(MediaDownloadType downloadType, openmedia::downloader::media_quality_type_t quality)
{
    QString result;

    switch (downloadType)
    {
    case ViewModel::DownloadVideo:
    {
        switch (quality)
        {
        case downloader::mediaQualityOriginal:
            result = QObject::tr("Original Quality");
            break;

        case downloader::mediaQuality1080P:
            result = QObject::tr("High Definition");
            break;
            
            
        case downloader::mediaQuality720P_1:
        case downloader::mediaQuality720P:
            result = QObject::tr("High Definition");
            break;

        case downloader::mediaQuality480P:
            result = QObject::tr("High Quality");
            break;

        case downloader::mediaQuality360P_1:
        case downloader::mediaQuality360P:
            result = QObject::tr("Normal Quality");
            break;

        case downloader::mediaQuality240P_1:
        case downloader::mediaQuality240P:
            result = QObject::tr("Normal Quality");
            break;
            
        case downloader::mediaQualityQCIF:
            result = QObject::tr("Low Resolution");
            break;            

        default:
            result = QObject::tr("Unknown");
            break;
        }

        break;
    }

    case ViewModel::ExtractAudio:
        result = QObject::tr("Original Audio");
        break;

    default:
        break;
    }

    return result;
}


QString MediaDownloadDetails::quality(MediaDownloadType downloadType, openmedia::downloader::media_quality_type_t quality)
{
    QString result;

    switch (downloadType)
    {
    case ViewModel::DownloadVideo:
    {
        switch (quality)
        {
        case downloader::mediaQualityOriginal:
            result = QObject::tr("4k");
            break;

        case downloader::mediaQuality1080P:
            result = "1080p";
            break;

        case downloader::mediaQuality720P_1:
        case downloader::mediaQuality720P:
            result = "720p";
            break;

        case downloader::mediaQuality480P:
            result = "480p";
            break;

        case downloader::mediaQuality360P_1:
        case downloader::mediaQuality360P:
            result = "360p";
            break;

        case downloader::mediaQuality240P_1:            
        case downloader::mediaQuality240P:
            result = "240p";
            break;
            
        case downloader::mediaQualityQCIF:
            result = "QCIF";
            break;            

        default:
            result = QObject::tr("Unknown");
            break;
        }

        break;
    }

    case ViewModel::ExtractAudio:
        result = QObject::tr("MP3");
        break;

    default:
        break;
    }

    return result;
}


QString MediaDownloadDetails::format(MediaDownloadType downloadType, openmedia::downloader::media_content_type_t format)
{
    QString result;

    switch (downloadType)
    {
    case ViewModel::DownloadVideo:
    {
        switch (format)
        {
        case downloader::mediaContentVideoFlv:
            result = "FLV";
            break;

        case downloader::mediaContentVideoMP4:
            result = "MP4";
            break;

        case downloader::mediaContentVideoWebm:
            result = "MKV";
            break;
            
        case downloader::mediaContentVideo3GP:
            result = "3GP";
            break;            

        default:
            result = QObject::tr("Unknown");
            break;
        }

        break;
    }

    case ViewModel::ExtractAudio:
        result = "MP3";
        break;

    default:
        break;
    }

    return result;
}


QString MediaDownloadDetails::codecs(MediaDownloadType downloadType, openmedia::downloader::media_content_type_t format, openmedia::downloader::media_video_type_t video, openmedia::downloader::media_audio_type_t audio)
{
    QString result;

    switch (downloadType)
    {
    case ViewModel::DownloadVideo:
    {
        switch (format)
        {
        case downloader::mediaContentVideoFlv:
            result += "FLV";
            break;

        case downloader::mediaContentVideoMP4:
            result += "MP4";
            break;

        case downloader::mediaContentVideoWebm:
            result += "MKV";
            break;
            
        case downloader::mediaContentVideo3GP:
            result += "3GP";
            break;

        default:
            result += QObject::tr("Unknown container");
            break;
        }

        result += " / ";

        switch (video)
        {
        case downloader::mediaVideoH264:
            result += "H264";
            break;

        case downloader::mediaVideoH263:
            result += "H263";
            break;

        case downloader::mediaVideoMPEG4:
            result += "MPEG4";
            break;

        case downloader::mediaVideoVP8:
            result += "VP8";
            break;

        default:
            result += QObject::tr("Unknown video");
            break;
        }

        result += " / ";

        switch (audio)
        {
        case downloader::mediaAudioAAC:
            result += "AAC";
            break;

        case downloader::mediaAudioMP3:
            result += "MP3";
            break;

        case downloader::mediaAudioAMR:
            result += "AMR";
            break;

        case downloader::mediaAudioVorbis:
            result += "Vorbis";
            break;

        default:
            result += QObject::tr("Unknown audio");
            break;
        }

        break;
    }

    case ViewModel::ExtractAudio:
        result = "MP3";
        break;

    default:
        break;
    }

    return result;
}


QString MediaDownloadDetails::fileExt(MediaDownloadType downloadType, openmedia::downloader::media_content_type_t format)
{
    QString result;

    switch (downloadType)
    {
    case ViewModel::DownloadVideo:
    {
        switch (format)
        {
        case downloader::mediaContentVideoFlv:
            result = ".flv";
            break;

        case downloader::mediaContentVideoMP4:
            result = ".mp4";
            break;

        case downloader::mediaContentVideoWebm:
            result = ".mkv";
            break;
            
        case downloader::mediaContentVideo3GP:
            result = ".3gp";
            break;            

        default:
            result = ".flv";
            break;
        }

        break;
    }

    case ViewModel::ExtractAudio:
        result = ".mp3";
        break;

    default:
        break;
    }

    return result;
}


QString MediaDownloadDetails::summary(MediaDownloadType downloadType, const openmedia::downloader::media_download_list_ptr mediaList, int mediaIndex)
{
    return summary(downloadType, mediaList->media(mediaIndex).quality());
}


QString MediaDownloadDetails::quality(MediaDownloadType downloadType, const openmedia::downloader::media_download_list_ptr mediaList, int mediaIndex)
{
    return quality(downloadType, mediaList->media(mediaIndex).quality());
}


QString MediaDownloadDetails::format(MediaDownloadType downloadType, const openmedia::downloader::media_download_list_ptr mediaList, int mediaIndex)
{
    return format(downloadType, mediaList->media(mediaIndex).content_type());
}


QString MediaDownloadDetails::codecs(MediaDownloadType downloadType, const openmedia::downloader::media_download_list_ptr mediaList, int mediaIndex)
{
    return codecs(downloadType, mediaList->media(mediaIndex).content_type(), mediaList->media(mediaIndex).video_codec(), mediaList->media(mediaIndex).audio_codec());
}


QString MediaDownloadDetails::fileExt(MediaDownloadType downloadType, const openmedia::downloader::media_download_list_ptr mediaList, int mediaIndex)
{
    return fileExt(downloadType, mediaList->media(mediaIndex).content_type());
}


QString MediaDownloadDetails::fileSize(MediaDownloadType downloadType, const openmedia::downloader::media_download_list_ptr mediaList, int mediaIndex)
{
    QString result;
    downloader::media_download_info media = mediaList->media(mediaIndex);

    switch (downloadType)
    {
    case ViewModel::DownloadVideo:
        result = Transform::storageSizeToString(qint64(media.content_size()));
        break;

    case ViewModel::ExtractAudio:
        result = Transform::storageSizeToString(qint64(mediaList->duration()) * 128 * 1024 / (8 * 1000));
        break;

    default:
        break;
    }

    return result;
}



MediaDownloadItem::MediaDownloadItem() :
    m_downloadType(ViewModel::DownloadVideo),
    m_mediaIndex(-1),
    m_subtitlesIndex(-1),
    m_needGenerate(false)
{
}

MediaDownloadItem::MediaDownloadItem(MediaDownloadType downloadType, openmedia::downloader::media_download_list_ptr mediaList,
                                     int mediaIndex, int subtitlesIndex, QString outputPath, bool needGenerate) :
    m_downloadType(downloadType),
    m_mediaList(mediaList),
    m_mediaIndex(mediaIndex),
    m_subtitlesIndex(subtitlesIndex),
    m_outputPath(outputPath),
    m_needGenerate(needGenerate)
{
    qRegisterMetaType<ViewModel::MediaDownloadType>();
    qRegisterMetaType<ViewModel::MediaDownloadItem>();
    qRegisterMetaType<ViewModel::MediaDownloadList>();
}


MediaDownloadType MediaDownloadItem::downloadType() const
{
    return m_downloadType;
}


openmedia::downloader::media_download_list_ptr MediaDownloadItem::mediaList() const
{
    return m_mediaList;
}


openmedia::downloader::media_download_info MediaDownloadItem::media() const
{
    return m_mediaList->media(m_mediaIndex);
}


int MediaDownloadItem::mediaIndex() const
{
    return m_mediaIndex;
}


int MediaDownloadItem::subtitlesIndex() const
{
    return m_subtitlesIndex;
}


QString MediaDownloadItem::mediaFileName() const
{
    if (m_needGenerate)
    {
        QString fileExt = MediaDownloadDetails::fileExt(m_downloadType, m_mediaList, m_mediaIndex);
        QString title = QString::fromStdWString(m_mediaList->title());
        return FileSystem::generateUniqueFilePath(m_outputPath, title, fileExt);
    }

    return m_outputPath;
}


QString MediaDownloadItem::subtitlesFileName() const
{
    return FileSystem::changeFilePathExt(mediaFileName(), ".srt");
}

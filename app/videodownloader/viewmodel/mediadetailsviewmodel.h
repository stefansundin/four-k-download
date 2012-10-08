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


#ifndef MEDIADETAILSVIEWMODEL_H
#define MEDIADETAILSVIEWMODEL_H

#include <QObject>
#include "mvvm/cxx/dialog.h"
#include "componentmodel/notifyobject.h"
#include "viewmodel/mediadownload.h"


namespace ViewModel
{

class MediaDetailsViewModel : public ComponentModel::NotifyObject
{
    Q_OBJECT
    Q_PROPERTY(QString summary READ summary)
    Q_PROPERTY(QString quality READ quality)
    Q_PROPERTY(QString codecs READ codecs)
    Q_PROPERTY(QString format READ format)
    Q_PROPERTY(QString fileExt READ fileExt)
    Q_PROPERTY(QString fileSize READ fileSize)

public:
    MediaDetailsViewModel(const Mvvm::Dialog* dialog, MediaDownloadType downloadType,
                          openmedia::downloader::media_download_list_ptr mediaList, int mediaIndex,
                          QObject *parent = 0);
    MediaDetailsViewModel(const Mvvm::Dialog* dialog, MediaDownloadType downloadType,
                          openmedia::downloader::media_quality_type_t mediaQuality,
                          openmedia::downloader::media_content_type_t mediaContent,
                          const QString& summary, const QString& quality, const QString& codecs,
                          const QString& format, const QString& fileExt, const QString& fileSize,
                          QObject *parent = 0);

    MediaDownloadType downloadType() const;
    openmedia::downloader::media_quality_type_t mediaQuality() const;
    openmedia::downloader::media_content_type_t mediaContent() const;
    QString summary() const;
    QString quality() const;
    QString codecs() const;
    QString format() const;
    QString fileExt() const;
    QString fileSize() const;

private:
    QWeakPointer<const Mvvm::Dialog> m_dialog;
    MediaDownloadType m_downloadType;
    openmedia::downloader::media_quality_type_t m_mediaQuality;
    openmedia::downloader::media_content_type_t m_mediaContent;
    QString m_summary;
    QString m_quality;
    QString m_codecs;
    QString m_format;
    QString m_fileExt;
    QString m_fileSize;
};

} // ViewModel

#endif // MEDIADETAILSVIEWMODEL_H

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


#ifndef DOWMLOADITEMVIEWMODEL_H
#define DOWMLOADITEMVIEWMODEL_H

#include <QObject>
#include <QPixmap>
#include <QTime>
#include <QEvent>
#include <QAction>
#include "componentmodel/notifyobject.h"
#include "componentmodel/xmlserializable.h"
#include "componentmodel/progress.h"
#include "mvvm/cxx/dialog.h"
#include "openmedia/DTMediaDownloader.h"
#include "openmedia/DTVideo2Mp3.h"
#include "viewmodel/mediadownload.h"
#include "viewmodel/downloadsettingsviewmodel.h"


namespace ViewModel
{

class DownloadItemViewModel : public ComponentModel::NotifyObject, public ComponentModel::IXmlSerializable
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title)
    Q_PROPERTY(QPixmap thumbnail READ thumbnail)
    Q_PROPERTY(QString size READ size)
    Q_PROPERTY(QString duration READ duration)
    Q_PROPERTY(QString format READ format)
    Q_PROPERTY(Media media READ media)
    Q_PROPERTY(int progress READ progress)
    Q_PROPERTY(QString remainingTime READ remainingTime)
    Q_PROPERTY(State state READ state WRITE setState)
    Q_PROPERTY(QAction* basicAction READ basicAction)
    Q_PROPERTY(QAction* folderAction READ folderAction)
    Q_PROPERTY(QAction* urlAction READ urlAction)
    Q_PROPERTY(QAction* facebookAction READ facebookAction)
    Q_PROPERTY(QAction* twitterAction READ twitterAction)
    Q_PROPERTY(State state READ state WRITE setState)
    Q_ENUMS(Media)
    Q_ENUMS(State)

public:
    enum State { None, Parse, Download, Convert, Pause, Done, Error };
    enum Media { Video, Audio };

    DownloadItemViewModel(const Mvvm::Dialog* dialog, QObject* parent = 0);
    DownloadItemViewModel(const Mvvm::Dialog* dialog, ViewModel::MediaDownloadItem item, QObject* parent = 0);
    DownloadItemViewModel(const Mvvm::Dialog* dialog, ViewModel::DownloadSettingsViewModel* settings, QObject* parent = 0);
    virtual ~DownloadItemViewModel();

    virtual bool serialize(QDomDocument& doc, QDomElement& element) const;
    virtual bool deserialize(const QDomDocument& doc, const QDomElement& element);

    QString title() const;
    QPixmap thumbnail() const;
    QString size() const;
    QString duration() const;
    QString format() const;
    Media media() const;
    int progress() const;
    QString remainingTime() const;
    State state() const;
    QAction* basicAction();
    QAction* folderAction();
    QAction* urlAction();
    QAction* facebookAction();
    QAction* twitterAction();

signals:
    void parsed(ViewModel::MediaDownloadList downloadList);
    void downloadCompleted();

public slots:
    void pause();
    void resume();
    void showInFolder();
    void play();
    void copyUrl();
    void shareOnFacebook();
    void shareOnTwitter();

protected slots:
    void doBasicAction();
    void doRemainingTimeChanged(const ComponentModel::PropertyChangedSignalArgs& args);
    void doSettingsPropertyChanged(const ComponentModel::PropertyChangedSignalArgs& args);

protected:
    void initialize(ViewModel::MediaDownloadItem item);

    void setThumbnail(QPixmap value);
    void setProgress(int value);
    void setRemainingTime(qint64 value);
    void setState(State value);

    virtual bool event(QEvent* e);

private:
    QWeakPointer<const Mvvm::Dialog> m_dialog;
    QSharedPointer<ViewModel::DownloadSettingsViewModel> m_settings;
    QString m_downloadFileName;
    QString m_resultFileName;
    QString m_url;
    QPixmap m_thumbnail;
    qint64 m_size;
    qint32 m_duration;
    QString m_format;
    Media m_media;
    QScopedPointer<openmedia::downloader::media_downloader> m_downloader;
    QScopedPointer<ComponentModel::Progress> m_progressMetrics;
    int m_progress;
    qint64 m_remainingTime;
    State m_state;
    QAction m_basicAction;
    QAction m_folderAction;
    QAction m_urlAction;
    QAction m_facebookAction;
    QAction m_twitterAction;
};

} // ViewModel

Q_DECLARE_METATYPE(ViewModel::DownloadItemViewModel::State)
Q_DECLARE_METATYPE(ViewModel::DownloadItemViewModel::Media)

#endif // DOWMLOADITEMVIEWMODEL_H

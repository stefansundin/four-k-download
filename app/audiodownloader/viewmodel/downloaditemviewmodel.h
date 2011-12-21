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
#include "mvvm/dialog.h"
#include "openmedia/DTMediaDownloader.h"
#include "openmedia/DTVideo2Mp3.h"


namespace ViewModel
{

class DownloadItemViewModel : public ComponentModel::NotifyObject, public ComponentModel::IXmlSerializable
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title)
    Q_PROPERTY(QString size READ size)
    Q_PROPERTY(QString duration READ duration)
    Q_PROPERTY(int progress READ progress)
    Q_PROPERTY(QString remainingTime READ remainingTime)
    Q_PROPERTY(State state READ state WRITE setState)
    Q_PROPERTY(QAction* basicAction READ basicAction)
    Q_PROPERTY(QAction* folderAction READ folderAction)
    Q_PROPERTY(QAction* urlAction READ urlAction)
    Q_PROPERTY(QAction* facebookAction READ facebookAction)
    Q_PROPERTY(QAction* twitterAction READ twitterAction)
    Q_PROPERTY(State state READ state WRITE setState)
    Q_ENUMS(State)

public:
    enum State { None, Parse, Initialized, Download, Convert, Pause, Done, Error };

    DownloadItemViewModel(const Mvvm::Dialog* dialog, QObject* parent = 0);
    DownloadItemViewModel(const Mvvm::Dialog* dialog, QString url, bool isPlaylist, QObject* parent = 0);
    DownloadItemViewModel(const Mvvm::Dialog* dialog, openmedia::downloader::media_download_list_ptr item, QObject* parent = 0);
    virtual ~DownloadItemViewModel();

    virtual bool serialize(QDomDocument& doc, QDomElement& element) const;
    virtual bool deserialize(const QDomDocument& doc, const QDomElement& element);

    QString title() const;
    QString duration() const;
    QString size() const;
    int progress() const;
    QString remainingTime() const;
    State state() const;
    QAction* basicAction();
    QAction* folderAction();
    QAction* urlAction();
    QAction* facebookAction();
    QAction* twitterAction();

signals:
    void parsed(openmedia::downloader::url_parser_result_ptr result);

public slots:
    void pause();
    void resume();
    void showInFolder();
    void play();
    void copyUrl();
    void shareOnFacebook();
    void shareOnTwitter();

    void doBasicAction();
    void doRemainingTimeChanged(const ComponentModel::PropertyChangedSignalArgs& args);

protected:
    void setProgress(int value);
    void setRemainingTime(qint64 value);
    void setState(State value);

    void doParsed(openmedia::downloader::url_parser_result_ptr result, openmedia::downloader::url_parser::ErrorCode error);
    void doInitialized(openmedia::downloader::media_download_list::InitializedResult result);

    virtual bool event(QEvent* e);

private:
    QWeakPointer<const Mvvm::Dialog> m_dialog;
    QString m_downloadFileName;
    QString m_resultFileName;
    QString m_url;
    qint64 m_size;
    qint32 m_duration;
    QScopedPointer<openmedia::downloader::media_downloader> m_downloader;
    openmedia::downloader::media_download_list_ptr m_item;
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

#endif // DOWMLOADITEMVIEWMODEL_H

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
#include "componentmodel/objectlessor.h"
#include "multimedia/audioplayer.h"
#include "mvvm/cxx/dialog.h"
#include "openmedia/DTMediaDownloader.h"
#include "openmedia/DTVideo2Mp3.h"
#include "openmedia/DTAVSource.h"


namespace ViewModel
{

class DownloadItemViewModel : public ComponentModel::NotifyObject, public ComponentModel::IXmlSerializable, public ComponentModel::IObjectLessee
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title)
    Q_PROPERTY(QString size READ size)
    Q_PROPERTY(QString duration READ duration)
    Q_PROPERTY(int progress READ progress)
    Q_PROPERTY(QString remainingTime READ remainingTime)
    Q_PROPERTY(QString playingTime READ playingTime)
    Q_PROPERTY(State state READ state WRITE setState)
    Q_PROPERTY(QAction* basicAction READ basicAction)
    Q_PROPERTY(QAction* urlAction READ urlAction)
    Q_PROPERTY(QAction* facebookAction READ facebookAction)
    Q_PROPERTY(QAction* twitterAction READ twitterAction)
    Q_PROPERTY(QAction* togglePlayAction READ togglePlayAction)
    Q_PROPERTY(State state READ state WRITE setState)
    Q_ENUMS(State)

public:
    enum State { NoneState, ParseState, InitializedState, DownloadState, ConvertState, PauseState, DoneState, ErrorState, PlayerPlayState, PlayerPauseState };

    DownloadItemViewModel(const Mvvm::Dialog* dialog, QSharedPointer<ComponentModel::ObjectLessor> playerLessor, QObject* parent = 0);
    DownloadItemViewModel(const Mvvm::Dialog* dialog, QString url, bool isPlaylist,
                          QSharedPointer<ComponentModel::ObjectLessor> playerLessor, QObject* parent = 0);
    DownloadItemViewModel(const Mvvm::Dialog* dialog, openmedia::downloader::media_download_list_ptr item,
                          QSharedPointer<ComponentModel::ObjectLessor> playerLessor, QObject* parent = 0);
    virtual ~DownloadItemViewModel();

    virtual bool serialize(QDomDocument& doc, QDomElement& element) const;
    virtual bool deserialize(const QDomDocument& doc, const QDomElement& element);

    QString title() const;
    QString duration() const;
    QString size() const;
    int progress() const;
    QString remainingTime() const;
    QString playingTime() const;
    State state() const;
    QAction* basicAction();
    QAction* urlAction();
    QAction* facebookAction();
    QAction* twitterAction();
    QAction* togglePlayAction();

public slots:
    void pause();
    void resume();
    void showInFolder();
    void copyUrl();
    void shareOnFacebook();
    void shareOnTwitter();
    void togglePlay();

signals:
    void parsed(openmedia::downloader::url_parser_result_ptr result);
    void downloadCompleted();
    void playerCompleted();

protected:
    void setProgress(int value);
    void setRemainingTime(qint64 value);
    void setState(State value);
    void leasePlayer();
    void releasePlayer(bool notifyLessor);

    void doParsed(openmedia::downloader::url_parser_result_ptr result, openmedia::downloader::url_parser::ErrorCode error);
    void doInitialized(openmedia::downloader::media_download_list::InitializedResult result);

    virtual bool releaseLeasableObject(ComponentModel::ObjectLessor* lessor);
    virtual bool event(QEvent* e);

protected slots:
    void onBasicAction();
    void onRemainingTimeChanged(const ComponentModel::PropertyChangedSignalArgs& args);
    void onPlayerTimeChanged(qint64 playerTime);
    void onPlayerStateChanged(Multimedia::Player::State playerState);
    void onPlayerCompleted();

private:
    QWeakPointer<const Mvvm::Dialog> m_dialog;
    QSharedPointer<ComponentModel::ObjectLessor> m_playerLessor;
    QWeakPointer<Multimedia::AudioPlayer> m_player;
    QScopedPointer<ComponentModel::Progress> m_progressMetrics;
    QScopedPointer<openmedia::downloader::media_downloader> m_downloader;
    openmedia::downloader::media_download_list_ptr m_item;
    openmedia::av_source_ptr m_source;
    QString m_downloadFileName;
    QString m_resultFileName;
    QString m_url;
    qint64 m_size;
    qint32 m_duration;
    int m_progress;
    qint64 m_remainingTime;
    State m_state;
    QAction m_basicAction;
    QAction m_urlAction;
    QAction m_facebookAction;
    QAction m_twitterAction;
    QAction m_togglePlayAction;
};

} // ViewModel

Q_DECLARE_METATYPE(ViewModel::DownloadItemViewModel::State)

#endif // DOWMLOADITEMVIEWMODEL_H

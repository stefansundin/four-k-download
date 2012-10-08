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


#ifndef MEDIAITEMVIEWMODEL_H
#define MEDIAITEMVIEWMODEL_H

#include <QObject>
#include <QSharedPointer>
#include <QMetaType>
#include <QtDeclarative>
#include <QtXml>
#include "componentmodel/imageprovider.h"
#include "componentmodel/progress.h"
#include "componentmodel/objectlessor.h"
#include "multimedia/audioplayer.h"
#include "openmedia/DTAVSource.h"
#include <boost/shared_ptr.hpp>

namespace openmedia { namespace audio {
class video2mp3_control;
typedef boost::shared_ptr<video2mp3_control> video2mp3_control_ptr;
} }


namespace ViewModel
{

class MediaItemViewModel : public QObject, ComponentModel::IObjectLessee
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
    Q_PROPERTY(qint64 duration READ duration NOTIFY durationChanged)
    Q_PROPERTY(QString thumbnail READ thumbnail NOTIFY thumbnailChanged)
    Q_PROPERTY(int progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(qint64 remainingTime READ remainingTime NOTIFY remainingTimeChanged)
    Q_PROPERTY(qint64 playingTime READ playingTime NOTIFY playingTimeChanged)
    Q_PROPERTY(State state READ state NOTIFY stateChanged)
    Q_PROPERTY(Error error READ error NOTIFY errorChanged)
    Q_PROPERTY(QAction* showInputAction READ showInputAction NOTIFY fakeSignal)
    Q_PROPERTY(QAction* showOutputAction READ showOutputAction NOTIFY fakeSignal)
    Q_PROPERTY(QAction* pauseAction READ pauseAction NOTIFY fakeSignal)
    Q_PROPERTY(QAction* resumeAction READ resumeAction NOTIFY fakeSignal)
    Q_PROPERTY(QAction* togglePlayAction READ togglePlayAction NOTIFY fakeSignal)
    Q_ENUMS(State)
    Q_ENUMS(Error)

public:
    enum State { NoState, OpenState, ConvertState, PauseState, DoneState, ErrorState, PlayerPlayState, PlayerPauseState };
    enum Error { NoError, InvalidInputError, CantCreateFileError };

    MediaItemViewModel(QString inputPath,
                       QString outputDirPath,
                       QSharedPointer<ComponentModel::ImageProvider> imageProvider,
                       QSharedPointer<ComponentModel::ObjectLessor> playerLessor,
                       QObject *parent = 0);

    MediaItemViewModel(QSharedPointer<ComponentModel::ImageProvider> imageProvider,
                       QSharedPointer<ComponentModel::ObjectLessor> playerLessor,
                       QObject *parent = 0);
    virtual ~MediaItemViewModel();

    void initialize();

    QString title() const;
    QString inputPath() const;
    QString outputPath() const;
    qint64 duration() const;
    QString thumbnail() const;
    int progress() const;
    qint64 remainingTime() const;
    qint64 playingTime() const;
    State state() const;
    Error error() const;
    QAction* showInputAction();
    QAction* showOutputAction();
    QAction* pauseAction();
    QAction* resumeAction();
    QAction* togglePlayAction();

    bool serialize(QDomDocument& doc, QDomElement& element) const;
    bool deserialize(const QDomDocument& doc, const QDomElement& element);

public slots:
    void showInput() const;
    void showOutput() const;
    void pause() const;
    void resume() const;
    void togglePlay();

signals:
    void titleChanged(QString value);
    void durationChanged(qint64 value);
    void thumbnailChanged(QString value);
    void progressChanged(int value);
    void remainingTimeChanged(qint64 value);
    void playingTimeChanged(qint64 value);
    void stateChanged(ViewModel::MediaItemViewModel::State value);
    void errorChanged(ViewModel::MediaItemViewModel::Error value);
    void fakeSignal(); // Disable qml warnings
    void conversionCompleted();
    void playerCompleted();

protected:
    void setThumbnail(QPixmap value);
    void setDuration(qint64 value);
    void setState(State value);
    void setProgress(int value);
    void leasePlayer();
    void releasePlayer(bool notifyLessor);

    virtual bool releaseLeasableObject(ComponentModel::ObjectLessor* lessor);
    virtual bool event(QEvent* e);

protected slots:
    void onRemainingTimeChanged(const ComponentModel::PropertyChangedSignalArgs& args);
    void onPlayerTimeChanged(qint64 playerTime);
    void onPlayerStateChanged(Multimedia::Player::State playerState);
    void onPlayerCompleted();

private:
    QSharedPointer<ComponentModel::ImageProvider> m_imageProvider;
    QSharedPointer<ComponentModel::ObjectLessor> m_playerLessor;
    QWeakPointer<Multimedia::AudioPlayer> m_player;
    QScopedPointer<ComponentModel::Progress> m_progressMetrics;
    QString m_title;
    QString m_inputPath;
    QString m_outputDirPath;
    QString m_outputPath;
    QString m_thumbnail;
    qint64 m_duration;
    int m_progress;
    State m_state;
    Error m_error;
    QAction m_showInputAction;
    QAction m_showOutputAction;
    QAction m_pauseAction;
    QAction m_resumeAction;
    QAction m_togglePlayAction;
    openmedia::audio::video2mp3_control_ptr m_control;
    openmedia::av_source_ptr m_source;
};

} // ViewModel

Q_DECLARE_METATYPE(ViewModel::MediaItemViewModel::State)
Q_DECLARE_METATYPE(ViewModel::MediaItemViewModel::Error)

#endif // MEDIAITEMVIEWMODEL_H

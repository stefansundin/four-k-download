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


#include "viewmodel/mediaitemviewmodel.h"

#include "componentmodel/notifyobject.h"
#include "componentmodel/filesystem.h"
#include "componentmodel/transform.h"
#include "componentmodel/filesystem.h"

#include <openmedia/DTMediaFile.h>
#include <openmedia/DTFileInfo.h>
#include <openmedia/DTVideoData.h>
#include <openmedia/DTVideo2Mp3.h>

#include <iostream>

using namespace ViewModel;
using namespace ComponentModel;
using namespace Multimedia;
using namespace openmedia;


namespace
{

const QEvent::Type OpenMediaFileEventType = QEvent::Type(QEvent::User + 1);
const QEvent::Type ReceiveThumbnailEventType = QEvent::Type(QEvent::User + 2);
const QEvent::Type ConvertStateEventType = QEvent::Type(QEvent::User + 3);
const QEvent::Type GetControlEventType = QEvent::Type(QEvent::User + 4);

QMutex mutex;


class OpenEvent : public QEvent
{
public:
    OpenEvent(media_file::OpenResult result, media_handle_ptr mediaHandle) :
        QEvent(OpenMediaFileEventType),
        m_result(result),
        m_mediaHandle(mediaHandle)
    {}

    media_file::OpenResult result() { return m_result; }
    media_handle_ptr mediaHandle() { return m_mediaHandle; }

private:
    media_file::OpenResult m_result;
    media_handle_ptr m_mediaHandle;
};

class OpenFunctor
{
public:
    OpenFunctor(QObject* item) :
            m_item(item)
    {
    }

    void operator ()(media_file::OpenResult result, media_handle_ptr mediaHandle)
    {
        if (!m_item.isNull())
            QCoreApplication::postEvent(m_item.data(), new OpenEvent(result, mediaHandle));
    }

private:
    QPointer<QObject> m_item;
};

class ReceiveThumbnailEvent : public QEvent
{
public:
    ReceiveThumbnailEvent(media_handle_ptr mediaHandle,
                          media_file::OpenResult result,
                          video_data_ptr videoData) :
        QEvent(ReceiveThumbnailEventType),
        m_mediaHandle(mediaHandle),
        m_result(result),
        m_videoData(videoData)
    {}

    media_handle_ptr mediaHandle() { return m_mediaHandle; }
    media_file::OpenResult result() { return m_result; }
    video_data_ptr videoData() { return m_videoData; }

private:
    media_handle_ptr m_mediaHandle;
    media_file::OpenResult m_result;
    video_data_ptr m_videoData;
};

class ReceiveThumbnailFunctor
{
public:
    ReceiveThumbnailFunctor(QObject* item, media_handle_ptr mediaHandle) :
        m_item(item),
        m_mediaHandle(mediaHandle)
    {
    }

    void operator()(media_file::OpenResult result, video_data_ptr videoData)
    {
        if (!m_item.isNull())
            QCoreApplication::postEvent(m_item.data(), new ReceiveThumbnailEvent(m_mediaHandle, result, videoData));
    }

private:
    QPointer<QObject> m_item;
    media_handle_ptr m_mediaHandle;

};

class ConvertStateEvent : public QEvent
{
public:
    ConvertStateEvent(audio::video2mp3::State state, double progress) :
            QEvent(ConvertStateEventType),
            m_state(state),
            m_progress(progress)
    {
    }

    audio::video2mp3::State state()
    {
        return m_state;
    }

    double progress()
    {
        return m_progress;
    }

private:
    audio::video2mp3::State m_state;
    double m_progress;
};

class ConvertStateFunctor
{
public:
    ConvertStateFunctor(QObject* item) :
            m_item(item)
    {
    }

    bool operator () (audio::video2mp3::State state, double progress)
    {
        if (!m_item.isNull())
            QCoreApplication::postEvent(m_item.data(), new ConvertStateEvent(state, progress));
        return true;
    }

private:
    QPointer<QObject> m_item;
};

class GetControlEvent : public QEvent
{
public:
    GetControlEvent(audio::video2mp3_control_ptr control) :
        QEvent(GetControlEventType),
        control_(control)
    {}

    audio::video2mp3_control_ptr control() { return control_; }

private:
    audio::video2mp3_control_ptr control_;

};

class GetControlFunctor
{
public:
    GetControlFunctor(QObject* item) :
        m_item(item)
    {
    }

    bool operator () (audio::video2mp3_control_ptr control)
    {
        if (!m_item.isNull())
            QCoreApplication::postEvent(m_item.data(), new GetControlEvent(control));
        return true;
    }

private:
    QPointer<QObject> m_item;
};

} // Anonimous


MediaItemViewModel::MediaItemViewModel(QString inputPath,
                                       QString outputDirPath,
                                       QSharedPointer<ComponentModel::ImageProvider> imageProvider,
                                       QSharedPointer<ComponentModel::ObjectLessor> playerLessor,
                                       QObject *parent) :
    QObject(parent),
    m_imageProvider(imageProvider),
    m_playerLessor(playerLessor),
    m_title("Title Text"),
    m_inputPath(inputPath),
    m_outputDirPath(outputDirPath),
    m_outputPath(),
    m_thumbnail(""),
    m_duration(0),
    m_progress(-1),
    m_state(NoState),
    m_error(NoError),
    m_showInputAction(this),
    m_showOutputAction(this),
    m_pauseAction(this),
    m_resumeAction(this),
    m_togglePlayAction(this)
{
    initialize();
    media_file::open_async( std::string(inputPath.toUtf8().constData()), OpenFunctor(this) );

    QFileInfo fileInfo(inputPath);
    m_title = fileInfo.completeBaseName();

    QDir d;
    if (!d.exists(outputDirPath))
        d.mkpath(outputDirPath);

    mutex.lock();

    try
    {
        m_outputPath = FileSystem::generateUniqueFilePath(outputDirPath, m_title, ".mp3");
        QFile file(m_outputPath);
        if (file.open(QIODevice::WriteOnly))
            file.close();
    }
    catch(...)
    {
    }

    mutex.unlock();
}


MediaItemViewModel::MediaItemViewModel(QSharedPointer<ComponentModel::ImageProvider> imageProvider,
                                       QSharedPointer<ComponentModel::ObjectLessor> playerLessor,
                                       QObject *parent) :
    QObject(parent),
    m_imageProvider(imageProvider),
    m_playerLessor(playerLessor),
    m_title(),
    m_inputPath(),
    m_outputDirPath(),
    m_outputPath(),
    m_thumbnail(""),
    m_duration(0),
    m_progress(-1),
    m_state(NoState),
    m_error(NoError),
    m_showInputAction(this),
    m_showOutputAction(this),
    m_pauseAction(this),
    m_resumeAction(this),
    m_togglePlayAction(this)
{
    initialize();
}


void MediaItemViewModel::initialize()
{
    qRegisterMetaType<State>();
    qRegisterMetaType<Error>();

    m_showInputAction.setEnabled(true);
    QObject::connect(&m_showInputAction, SIGNAL(triggered()), this, SLOT(showInput()));

    m_showOutputAction.setEnabled(false);
    QObject::connect(&m_showOutputAction, SIGNAL(triggered()), this, SLOT(showOutput()));

    m_pauseAction.setEnabled(false);
    QObject::connect(&m_pauseAction, SIGNAL(triggered()), this, SLOT(pause()));

    m_resumeAction.setEnabled(false);
    QObject::connect(&m_resumeAction, SIGNAL(triggered()), this, SLOT(resume()));

    m_togglePlayAction.setEnabled(false);
    QObject::connect(&m_togglePlayAction, SIGNAL(triggered()), this, SLOT(togglePlay()));
}


MediaItemViewModel::~MediaItemViewModel()
{
    if (m_control)
        m_control->cancel();

    releasePlayer(true);
}


bool MediaItemViewModel::serialize(QDomDocument& doc, QDomElement& element) const
{
    element = doc.createElement("Item");

    if (!m_thumbnail.isEmpty())
    {
        QPixmap pixMap = m_imageProvider->requestPixmap(m_thumbnail, NULL, QSize());
        element.setAttribute("thumbnail", Transform::pixmapToBase64(pixMap));
    }


    element.setAttribute("title", m_title);
    element.setAttribute("inputPath", m_inputPath);
    element.setAttribute("outputPath", m_outputPath);
    element.setAttribute("duration", m_duration);

    return true;
}


bool MediaItemViewModel::deserialize(const QDomDocument& doc, const QDomElement& element)
{
    m_outputPath = element.attribute("outputPath");

    QFileInfo info(m_outputPath);
    if (!info.exists())
        return false;

    m_title = element.attribute("title");
    m_duration = element.attribute("duration").toInt();
    m_inputPath = element.attribute("inputPath");

    setThumbnail(Transform::base64ToPixmap(element.attribute("thumbnail")));
    setState(MediaItemViewModel::DoneState);
    return true;
}


QString MediaItemViewModel::title() const
{
    return m_title;
}


QString MediaItemViewModel::inputPath() const
{
    return m_inputPath;
}


QString MediaItemViewModel::outputPath() const
{
    return m_outputPath;
}


qint64 MediaItemViewModel::duration() const
{
    return m_duration;
}


QString MediaItemViewModel::thumbnail() const
{
    return m_thumbnail;
}


int MediaItemViewModel::progress() const
{
    return m_progress;
}


qint64 MediaItemViewModel::remainingTime() const
{
    if (m_progressMetrics)
        return m_progressMetrics->remainingTime();

    return -1;
}


qint64 MediaItemViewModel::playingTime() const
{
    if (m_player)
        return m_player.data()->time();

    return 0;
}


MediaItemViewModel::State MediaItemViewModel::state() const
{
    return m_state;
}


MediaItemViewModel::Error MediaItemViewModel::error() const
{
    return m_error;
}


QAction* MediaItemViewModel::showInputAction()
{
    return &m_showInputAction;
}


QAction* MediaItemViewModel::showOutputAction()
{
    return &m_showOutputAction;
}


QAction* MediaItemViewModel::pauseAction()
{
    return &m_pauseAction;
}


QAction* MediaItemViewModel::resumeAction()
{
    return &m_resumeAction;
}


QAction* MediaItemViewModel::togglePlayAction()
{
    return &m_togglePlayAction;
}


void MediaItemViewModel::setThumbnail(QPixmap value)
{
    if (m_thumbnail.isEmpty())
        m_thumbnail = QUuid::createUuid().toString();

    m_imageProvider->updatePixmap(m_thumbnail, value);

    emit thumbnailChanged(m_thumbnail);
}


void MediaItemViewModel::showInput() const
{
    FileSystem::showFile(inputPath());
}


void MediaItemViewModel::showOutput() const
{
    FileSystem::showFile(outputPath());
}


void MediaItemViewModel::pause() const
{
    if (m_control)
        m_control->pause();
}


void MediaItemViewModel::resume() const
{
    if (m_control)
        m_control->resume();
}


void MediaItemViewModel::togglePlay()
{
    switch (state())
    {
    case DoneState:
    case PlayerPauseState:
    {
        leasePlayer();

        if (!m_player.isNull())
            m_player.data()->play();

        break;
    }

    case PlayerPlayState:
    {
        if (!m_player.isNull())
            m_player.data()->pause();

        break;
    }

    default:
        break;
    }
}


void MediaItemViewModel::setDuration(qint64 value)
{
    if (value != m_duration)
    {
        m_duration = value;
        emit durationChanged(m_duration);
    }
}


void MediaItemViewModel::setState(MediaItemViewModel::State value)
{
    if (value != m_state)
    {
        m_state = value;

        if (m_progressMetrics)
        {
            if (m_state == ConvertState)
                m_progressMetrics->start();
            else
                m_progressMetrics->stop();
        }

        if (m_state == DoneState || m_state == ErrorState)
        {
            m_progressMetrics.reset();
        }

        emit stateChanged(value);

        m_showInputAction.setEnabled(true);
        m_showOutputAction.setEnabled(m_state == DoneState || m_state == PlayerPlayState || m_state == PlayerPauseState);
        m_pauseAction.setEnabled(m_state == ConvertState);
        m_resumeAction.setEnabled(m_state == PauseState);
        m_togglePlayAction.setEnabled(m_state == DoneState || m_state == PlayerPlayState || m_state == PlayerPauseState);
    }
}


void MediaItemViewModel::setProgress(int value)
{
    if (value != m_progress)
    {
        m_progress = value;
        emit progressChanged(m_progress);
    }
}


void MediaItemViewModel::leasePlayer()
{
    if (!m_playerLessor.isNull() && m_player.isNull())
    {
        m_player = static_cast<AudioPlayer*>(m_playerLessor->leaseObject(this));
        m_source = av_source_creator::create(outputPath().toUtf8().data());

        if (!m_player.isNull() && m_source.get())
        {
            m_player.data()->resetSource(m_source);

            QObject::connect(m_player.data(), SIGNAL(timeChanged(qint64)),
                             this, SLOT(onPlayerTimeChanged(qint64)));
            QObject::connect(m_player.data(), SIGNAL(stateChanged(Multimedia::Player::State)),
                             this, SLOT(onPlayerStateChanged(Multimedia::Player::State)));
            QObject::connect(m_player.data(), SIGNAL(completed()),
                             this, SLOT(onPlayerCompleted()));
        }

        emit playingTimeChanged(playingTime());
    }
}


void MediaItemViewModel::releasePlayer(bool notifyLessor)
{
    if (!m_player.isNull())
    {
        m_player.data()->resetSource();
        m_source.reset();

        QObject::disconnect(m_player.data(), SIGNAL(timeChanged(qint64)),
                           this, SLOT(onPlayerTimeChanged(qint64)));
        QObject::disconnect(m_player.data(), SIGNAL(stateChanged(Multimedia::Player::State)),
                            this, SLOT(onPlayerStateChanged(Multimedia::Player::State)));
        QObject::disconnect(m_player.data(), SIGNAL(completed()),
                            this, SLOT(onPlayerCompleted()));
        m_player.clear();

        if (notifyLessor && !m_playerLessor.isNull())
            m_playerLessor->releaseObject(this);

        emit playingTimeChanged(playingTime());
    }
}


bool MediaItemViewModel::releaseLeasableObject(ObjectLessor* lessor)
{
    if (!m_playerLessor.isNull() && m_playerLessor.data() == lessor)
    {
        releasePlayer(false);
        return true;
    }

    return false;
}


bool MediaItemViewModel::event(QEvent* e)
{
    switch (e->type())
    {
    case OpenMediaFileEventType:
        {
            OpenEvent * ce = (OpenEvent *)e;
            if (ce->result() == media_file::openSuccess && ce->mediaHandle())
            {
                media_file_info_ptr mediaInfo = media_file::get_media_file_info(ce->mediaHandle());
                file_info_general_ptr fileInfo = mediaInfo->get_file_info();
                const double duration =
                        1.0 * fileInfo->get_duration() * fileInfo->get_time_base().num / fileInfo->get_time_base().den;

                setDuration((qint64)(1000.0 * duration));

                m_progressMetrics.reset(new ComponentModel::Progress(0, 100000, 2000, 2000 ));
                QObject::connect(m_progressMetrics.data(), SIGNAL(remainingTimeChanged(ComponentModel::PropertyChangedSignalArgs)),
                                     this, SLOT(onRemainingTimeChanged(ComponentModel::PropertyChangedSignalArgs)));

                media_file::receive_thumbnail_async(ce->mediaHandle(),
                                                    ReceiveThumbnailFunctor(this, ce->mediaHandle()), -1, DT_PIX_FMT_RGB24, 160);
            }
            else
            {
                setState(ErrorState);
            }
        }
        return true;

    case ReceiveThumbnailEventType:
        {
            ReceiveThumbnailEvent * ce = (ReceiveThumbnailEvent *)e;
            if (ce->result() == media_file::openSuccess &&
                    ce->videoData() &&
                    ce->videoData()->get_data_size() > 0)
            {
                QImage image(ce->videoData()->get_data(),
                             ce->videoData()->get_width(),
                             ce->videoData()->get_height(),
                             ce->videoData()->get_line_size(0),
                             QImage::Format_RGB888
                             );
                setThumbnail(QPixmap::fromImage(image));
            }
            else
            {
                // TODO do nothing!!??
            }

            // TODO : Title
            audio::video2mp3::convert(ce->mediaHandle(),
                                      std::string(m_outputPath.toUtf8().constData()),
                                      m_title.toStdWString(),
                                      ConvertStateFunctor(this),
                                      GetControlFunctor(this), true);

           setState(ConvertState);
        }
        return true;

    case ConvertStateEventType:
        {
            ConvertStateEvent* ce = (ConvertStateEvent*)e;

            switch (ce->state())
            {
            case audio::video2mp3::stateProgress:
                if (m_progressMetrics)
                    m_progressMetrics->setValue((qint64)(100000.0 * ce->progress()));
                setProgress(ce->progress() * 100);
                break;

            case audio::video2mp3::stateFinish:
                if (m_control)
                    m_control.reset();
                setState(DoneState);
                emit conversionCompleted();
                break;

            case audio::video2mp3::statePause:
                setState(PauseState);
                break;

            case audio::video2mp3::stateResume:
                setState(ConvertState);
                break;

            default:
                if (m_control)
                    m_control.reset();
                setState(ErrorState);
                break;
            }
        }
        return true;

    case GetControlEventType:
        {
            GetControlEvent * ce = (GetControlEvent *)e;
            m_control = ce->control();
        }
        return true;

    default:
        return false;
    }
}


void MediaItemViewModel::onRemainingTimeChanged(const ComponentModel::PropertyChangedSignalArgs& args)
{
    Q_UNUSED(args)
    if (m_progressMetrics)
        emit remainingTimeChanged(m_progressMetrics->remainingTime());
}


void MediaItemViewModel::onPlayerTimeChanged(qint64 playerTime)
{
    emit playingTimeChanged(playerTime);
}


 void MediaItemViewModel::onPlayerStateChanged(Player::State playerState)
 {
     switch (playerState)
     {
     case Player::PlayState:
         setState(MediaItemViewModel::PlayerPlayState);
         break;

     case Player::PauseState:
         setState(MediaItemViewModel::PlayerPauseState);
         break;

     default:
         setState(MediaItemViewModel::DoneState);
         break;
     }
 }


 void MediaItemViewModel::onPlayerCompleted()
 {
     emit playerCompleted();
 }

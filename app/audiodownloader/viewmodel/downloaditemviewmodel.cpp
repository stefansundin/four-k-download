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


#include "viewmodel/downloaditemviewmodel.h"
#include "componentmodel/transform.h"
#include "componentmodel/filesystem.h"
#include <QFileInfo>
#include <QApplication>
#include <QPointer>
#include <QLocale>
#include <QDesktopServices>
#include <QUrl>
#include <QClipboard>
#include <QUuid>

#if defined(Q_OS_WIN)
#include <windows.h>
#endif

using namespace ViewModel;
using namespace ComponentModel;
using namespace Multimedia;
using namespace Mvvm;
using namespace openmedia;


namespace
{

const QEvent::Type DownloadStateEventType = QEvent::Type(QEvent::User + 1);
const QEvent::Type DownloadProgressEventType = QEvent::Type(QEvent::User + 2);
const QEvent::Type ConvertStateEventType = QEvent::Type(QEvent::User + 3);
const QEvent::Type ParseResultEventType = QEvent::Type(QEvent::User + 4);
const QEvent::Type InitializeResultEventType = QEvent::Type(QEvent::User + 5);

QMutex mutex;


class ParseResultEvent : public QEvent
{
public:
    ParseResultEvent(downloader::url_parser_result_ptr result, downloader::url_parser::ErrorCode error) :
            QEvent(ParseResultEventType),
            m_result(result),
            m_error(error)
    {
    }

    downloader::url_parser_result_ptr result()
    {
        return m_result;
    }

    downloader::url_parser::ErrorCode error()
    {
        return m_error;
    }

private:
    downloader::url_parser_result_ptr m_result;
    downloader::url_parser::ErrorCode m_error;
};


class ParseResultFunctor
{
public:
    ParseResultFunctor(QObject* item) :
            m_item(item)
    {
    }

    void operator () (downloader::url_parser_result_ptr result, downloader::url_parser::ErrorCode error)
    {
        if (!m_item.isNull())
            QCoreApplication::postEvent(m_item.data(), new ParseResultEvent(result, error));
    }

private:
    QPointer<QObject> m_item;
};


class InitializeResultEvent : public QEvent
{
public:
    InitializeResultEvent(downloader::media_download_list::InitializedResult result) :
            QEvent(InitializeResultEventType),
            m_result(result)
    {
    }

    downloader::media_download_list::InitializedResult result()
    {
        return m_result;
    }

private:
    downloader::media_download_list::InitializedResult m_result;
};


class InitializeResultFunctor
{
public:
    InitializeResultFunctor(QObject* item) :
            m_item(item)
    {
    }

    void operator () (downloader::media_download_list::InitializedResult result)
    {
        if (!m_item.isNull())
            QCoreApplication::postEvent(m_item.data(), new InitializeResultEvent(result));
    }

private:
    QPointer<QObject> m_item;
};


class DownloadStateEvent : public QEvent
{
public:
    DownloadStateEvent(downloader::media_downloader::download_state_t state) :
            QEvent(DownloadStateEventType),
            m_state(state)
    {
    }

    downloader::media_downloader::download_state_t state()
    {
        return m_state;
    }

private:
    downloader::media_downloader::download_state_t m_state;
};


class DownloadStateFunctor
{
public:
    DownloadStateFunctor(QObject* item) :
            m_item(item)
    {
    }

    void operator () (downloader::media_downloader::download_state_t state)
    {
        if (!m_item.isNull())
            QCoreApplication::postEvent(m_item.data(), new DownloadStateEvent(state));
    }

private:
    QPointer<QObject> m_item;
};


class DownloadProgressEvent : public QEvent
{
public:
    DownloadProgressEvent(quint64 progress) :
            QEvent(DownloadProgressEventType),
            m_progress(progress)
    {
    }

    quint64 progress()
    {
        return m_progress;
    }

private:
    quint64 m_progress;
};


class DownloadProgressFunctor
{
public:
    DownloadProgressFunctor(QObject* item) :
            m_item(item)
    {
    }

    void operator () (::boost::uint64_t progress)
    {
        if (!m_item.isNull())
            QCoreApplication::postEvent(m_item.data(), new DownloadProgressEvent(progress));
    }

private:
    QPointer<QObject> m_item;
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

} // Anonimous


DownloadItemViewModel::DownloadItemViewModel(const Mvvm::Dialog* dialog, QSharedPointer<ComponentModel::ObjectLessor> playerLessor, QObject* parent) :
    NotifyObject(parent),
    m_dialog(dialog),
    m_playerLessor(playerLessor),
    m_downloadFileName(""),
    m_resultFileName(""),
    m_url(""),
    m_size(0),
    m_duration(0),
    m_progress(-1),
    m_remainingTime(0),
    m_state(DownloadItemViewModel::NoneState),
    m_basicAction(this),
    m_urlAction(this),
    m_facebookAction(this),
    m_twitterAction(this),
    m_togglePlayAction(this)
{
    qRegisterMetaType<State>();

    m_basicAction.setEnabled(false);
    m_basicAction.setVisible(false);
    m_basicAction.setText("");
    m_basicAction.setToolTip("");
    QObject::connect(&m_basicAction, SIGNAL(triggered()), this, SLOT(onBasicAction()));

    m_urlAction.setEnabled(true);
    m_urlAction.setText(tr("Copy url"));
    m_urlAction.setToolTip(tr("Copty url address"));
    QObject::connect(&m_urlAction, SIGNAL(triggered()), this, SLOT(copyUrl()));

    m_facebookAction.setEnabled(true);
    m_facebookAction.setText(tr("Share on Facebook"));
    m_facebookAction.setToolTip(tr("Share link to clip on facebook"));
    m_facebookAction.setIcon(QIcon(":/image/item-facebook"));
    QObject::connect(&m_facebookAction, SIGNAL(triggered()), this, SLOT(shareOnFacebook()));

    m_twitterAction.setEnabled(true);
    m_twitterAction.setText(tr("Share on Twitter"));
    m_twitterAction.setToolTip(tr("Share link to clip on twitter"));
    m_twitterAction.setIcon(QIcon(":/image/item-twitter"));
    QObject::connect(&m_twitterAction, SIGNAL(triggered()), this, SLOT(shareOnTwitter()));

    m_togglePlayAction.setEnabled(false);
    m_togglePlayAction.setVisible(false);
    m_togglePlayAction.setText("");
    m_togglePlayAction.setToolTip("");
    QObject::connect(&m_togglePlayAction, SIGNAL(triggered()), this, SLOT(togglePlay()));
}


DownloadItemViewModel::DownloadItemViewModel(const Mvvm::Dialog* dialog, QString url, bool isPlaylist, QSharedPointer<ComponentModel::ObjectLessor> playerLessor, QObject* parent) :
    NotifyObject(parent),
    m_dialog(dialog),
    m_playerLessor(playerLessor),
    m_downloadFileName(""),
    m_resultFileName(""),
    m_url(url),
    m_size(0),
    m_duration(0),
    m_progress(-1),
    m_remainingTime(0),
    m_state(DownloadItemViewModel::NoneState),
    m_basicAction(this),
    m_urlAction(this),
    m_facebookAction(this),
    m_twitterAction(this),
    m_togglePlayAction(this)
{
    qRegisterMetaType<State>();

    m_basicAction.setEnabled(false);
    m_basicAction.setVisible(false);
    m_basicAction.setText("");
    m_basicAction.setToolTip("");
    QObject::connect(&m_basicAction, SIGNAL(triggered()), this, SLOT(onBasicAction()));

    m_urlAction.setEnabled(true);
    m_urlAction.setText(tr("Copy url"));
    m_urlAction.setToolTip(tr("Copty url address"));
    QObject::connect(&m_urlAction, SIGNAL(triggered()), this, SLOT(copyUrl()));

    m_facebookAction.setEnabled(true);
    m_facebookAction.setText(tr("Share on Facebook"));
    m_facebookAction.setToolTip(tr("Share link to clip on facebook"));
    m_facebookAction.setIcon(QIcon(":/image/item-facebook"));
    QObject::connect(&m_facebookAction, SIGNAL(triggered()), this, SLOT(shareOnFacebook()));

    m_twitterAction.setEnabled(true);
    m_twitterAction.setText(tr("Share on Twitter"));
    m_twitterAction.setToolTip(tr("Share link to clip on twitter"));
    m_twitterAction.setIcon(QIcon(":/image/item-twitter"));
    QObject::connect(&m_twitterAction, SIGNAL(triggered()), this, SLOT(shareOnTwitter()));

    m_togglePlayAction.setEnabled(false);
    m_togglePlayAction.setVisible(false);
    m_togglePlayAction.setText("");
    m_togglePlayAction.setToolTip("");
    QObject::connect(&m_togglePlayAction, SIGNAL(triggered()), this, SLOT(togglePlay()));

    setState(DownloadItemViewModel::ParseState);

    downloader::url_parser::parse_url_async(url.toStdString(),
                                            isPlaylist ? downloader::url_parser::parsePlaylist : downloader::url_parser::parseNormal,
                                            ParseResultFunctor(this));
}


DownloadItemViewModel::DownloadItemViewModel(const Mvvm::Dialog* dialog, openmedia::downloader::media_download_list_ptr item, QSharedPointer<ComponentModel::ObjectLessor> playerLessor, QObject* parent) :
    NotifyObject(parent),
    m_dialog(dialog),
    m_playerLessor(playerLessor),
    m_item(item),
    m_downloadFileName(""),
    m_resultFileName(""),
    m_url(""),
    m_size(0),
    m_duration(0),
    m_progress(-1),
    m_remainingTime(0),
    m_state(DownloadItemViewModel::NoneState),
    m_basicAction(this),
    m_urlAction(this),
    m_facebookAction(this),
    m_twitterAction(this),
    m_togglePlayAction(this)
{
    qRegisterMetaType<State>();

    m_basicAction.setEnabled(false);
    m_basicAction.setVisible(false);
    m_basicAction.setText("");
    m_basicAction.setToolTip("");
    QObject::connect(&m_basicAction, SIGNAL(triggered()), this, SLOT(onBasicAction()));

    m_urlAction.setEnabled(true);
    m_urlAction.setText(tr("Copy url"));
    m_urlAction.setToolTip(tr("Copty url address"));
    QObject::connect(&m_urlAction, SIGNAL(triggered()), this, SLOT(copyUrl()));

    m_facebookAction.setEnabled(true);
    m_facebookAction.setText(tr("Share on Facebook"));
    m_facebookAction.setToolTip(tr("Share link to clip on facebook"));
    m_facebookAction.setIcon(QIcon(":/image/item-facebook"));
    QObject::connect(&m_facebookAction, SIGNAL(triggered()), this, SLOT(shareOnFacebook()));

    m_twitterAction.setEnabled(true);
    m_twitterAction.setText(tr("Share on Twitter"));
    m_twitterAction.setToolTip(tr("Share link to clip on twitter"));
    m_twitterAction.setIcon(QIcon(":/image/item-twitter"));
    QObject::connect(&m_twitterAction, SIGNAL(triggered()), this, SLOT(shareOnTwitter()));

    m_togglePlayAction.setEnabled(false);
    m_togglePlayAction.setVisible(false);
    m_togglePlayAction.setText("");
    m_togglePlayAction.setToolTip("");
    QObject::connect(&m_togglePlayAction, SIGNAL(triggered()), this, SLOT(togglePlay()));

    m_url = QString::fromStdString(item->url());

    setState(DownloadItemViewModel::ParseState);

    m_item->initialize(InitializeResultFunctor(this));
}


DownloadItemViewModel::~DownloadItemViewModel()
{
    if (!m_downloadFileName.isEmpty())
        QFile::remove(m_downloadFileName);

    releasePlayer(true);
}


bool DownloadItemViewModel::serialize(QDomDocument& doc, QDomElement& element) const
{
    element = doc.createElement("Item");
    element.setAttribute("filename", m_resultFileName);
    element.setAttribute("url", m_url);
    element.setAttribute("duration", (int)m_duration);

    return true;
}


bool DownloadItemViewModel::deserialize(const QDomDocument& doc, const QDomElement& element)
{
    m_resultFileName = element.attribute("filename");

    QFileInfo info(m_resultFileName);
    if (!info.exists())
        return false;

    m_url = element.attribute("url");
    m_duration = element.attribute("duration").toInt();
    m_size = info.size();

    setState(DownloadItemViewModel::DoneState);

    return true;
}


QString DownloadItemViewModel::title() const
{
    switch (state())
    {
        case DownloadItemViewModel::ParseState:
            return tr("Retrieving information...");

        case DownloadItemViewModel::ErrorState:
            if (m_resultFileName.isEmpty())
                return m_url;

        default:
        {
            QFileInfo info(m_resultFileName);
            return info.completeBaseName();
        }
    }
}


QString DownloadItemViewModel::size() const
{
    return Transform::storageSizeToString(m_size);
}


QString DownloadItemViewModel::duration() const
{
    return Transform::timeToString(m_duration, "H:mm:ss");
}


int DownloadItemViewModel::progress() const
{
    return m_progress;
}


void DownloadItemViewModel::setProgress(int value)
{
    if (value != m_progress)
    {
        m_progress = value;
        emitPropertyChanged("progress", m_progress);
    }
}


QString DownloadItemViewModel::remainingTime() const
{
    if ((m_state == DownloadState || m_state == PauseState) && m_remainingTime > 0)
        return Transform::timeToString(m_remainingTime, "remain");

    return "";
}


void DownloadItemViewModel::setRemainingTime(qint64 value)
{
    if (value != m_remainingTime)
    {
        m_remainingTime = value;
        emitPropertyChanged("remainingTime", m_remainingTime);
    }
}


QString DownloadItemViewModel::playingTime() const
{
    if (m_player)
        return Transform::timeToString(m_player.data()->time(), "H:mm:ss");

    return "0:00:00";
}


DownloadItemViewModel::State DownloadItemViewModel::state() const
{
    return m_state;
}


void DownloadItemViewModel::setState(DownloadItemViewModel::State value)
{
    if (value != m_state)
    {
        m_state = value;


        if (m_progressMetrics)
        {
            if (m_state == DownloadState)
                m_progressMetrics->start();
            else
                m_progressMetrics->stop();
        }


        switch (m_state)
        {
        case ParseState:
        case InitializedState:
            m_basicAction.setEnabled(false);
            m_basicAction.setVisible(false);
            break;

        case DownloadState:
            m_basicAction.setEnabled(true);
            m_basicAction.setVisible(true);
            m_basicAction.setText(tr("Pause"));
            m_basicAction.setToolTip(tr("Pause download"));
            break;

        case ConvertState:
            m_basicAction.setEnabled(true);
            m_basicAction.setVisible(true);
            m_basicAction.setText(tr("Pause"));
            m_basicAction.setToolTip(tr("Pause convertion"));
            break;

        case PauseState:
            m_basicAction.setEnabled(true);
            m_basicAction.setVisible(true);
            m_basicAction.setText(tr("Resume"));
            m_basicAction.setToolTip(tr("Resume"));
            break;

        case DoneState:
            m_basicAction.setEnabled(true);
            m_basicAction.setVisible(true);
            m_basicAction.setText(tr("Show in folder"));
            m_basicAction.setToolTip(tr("Open folder with downloaded file"));
            m_togglePlayAction.setEnabled(true);
            m_togglePlayAction.setVisible(true);
            m_togglePlayAction.setIcon(QIcon(":/image/item-player-play"));

            m_progressMetrics.reset();
            break;

        case ErrorState:
            m_basicAction.setEnabled(false);
            m_basicAction.setVisible(false);
            m_togglePlayAction.setEnabled(false);
            m_togglePlayAction.setVisible(false);

            m_progressMetrics.reset();
            break;

        case PlayerPlayState:
            m_togglePlayAction.setEnabled(true);
            m_togglePlayAction.setVisible(true);
            m_togglePlayAction.setIcon(QIcon(":/image/item-player-pause"));
            break;

        case PlayerPauseState:
            m_togglePlayAction.setEnabled(true);
            m_togglePlayAction.setVisible(true);
            m_togglePlayAction.setIcon(QIcon(":/image/item-player-play"));
            break;

        default:
            break;
        }

        emitPropertyChanged("state", m_state);
        emitPropertyChanged("title", title());
        emitPropertyChanged("remainingTime", remainingTime());
        emitPropertyChanged("playingTime", playingTime());
    }
}


QAction* DownloadItemViewModel::basicAction()
{
    return &m_basicAction;
}


QAction* DownloadItemViewModel::urlAction()
{
    return &m_urlAction;
}


QAction* DownloadItemViewModel::facebookAction()
{
    return &m_facebookAction;
}


QAction* DownloadItemViewModel::twitterAction()
{
    return &m_twitterAction;
}


QAction* DownloadItemViewModel::togglePlayAction()
{
    return &m_togglePlayAction;
}


void DownloadItemViewModel::pause()
{
    if (m_state != DownloadState)
        return;

    m_downloader->pause();
}


void DownloadItemViewModel::resume()
{
    if (m_state != PauseState)
        return;

    m_downloader->resume();
}


void DownloadItemViewModel::showInFolder()
{
    if (m_state == DoneState || m_state == PlayerPlayState || m_state == PlayerPauseState)
        FileSystem::showFile(m_resultFileName);
}


void DownloadItemViewModel::copyUrl()
{
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(m_url);
}


void DownloadItemViewModel::shareOnFacebook()
{
}


void DownloadItemViewModel::shareOnTwitter()
{
}


void DownloadItemViewModel::togglePlay()
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


void DownloadItemViewModel::leasePlayer()
{
    if (!m_playerLessor.isNull() && m_player.isNull())
    {
        m_player = static_cast<AudioPlayer*>(m_playerLessor->leaseObject(this));
        m_source = av_source_creator::create(m_resultFileName.toUtf8().constData());

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

        emitPropertyChanged("playingTime", playingTime());
    }
}


void DownloadItemViewModel::releasePlayer(bool notifyLessor)
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

        emitPropertyChanged("playingTime", playingTime());
    }
}


int getDownloadIndex(openmedia::downloader::media_download_list_ptr list)
{
    for (int i = list->size() - 1; i >= 0 ; --i)
    {
        downloader::media_download_info info = list->media(i);

        if (info.quality() >= downloader::mediaQualityOriginal &&
            info.quality() <= downloader::mediaQuality360P &&
            info.audio_codec() != downloader::mediaAudioMP3)
            return i;
    }

    return -1;
}


void DownloadItemViewModel::doParsed(downloader::url_parser_result_ptr result, downloader::url_parser::ErrorCode error)
{
    if (!result || result->size() == 0)
        error = downloader::url_parser::errorUnknown;

    switch (error)
    {
    case downloader::url_parser::errorNone:
    {
        m_item = result->at(0);
        m_item->initialize(InitializeResultFunctor(this));

        emit parsed(result);
        break;
    }

    default:
        setState(DownloadItemViewModel::ErrorState);
        break;
    }
}


void DownloadItemViewModel::doInitialized(downloader::media_download_list::InitializedResult result)
{
    switch (result)
    {
    case downloader::media_download_list::initAlready:
    case downloader::media_download_list::initSuccess:
    {
        int index = getDownloadIndex(m_item);
        if (index == -1)
        {
            setState(DownloadItemViewModel::ErrorState);
            break;
        }

        m_size = qint64(m_item->media(index).content_size());
        m_duration = qint32(m_item->duration());

        QSettings settings;
        QString defaultDir = QDesktopServices::storageLocation(QDesktopServices::MusicLocation);
        QString outputDir = settings.value("Download/outputDirName", defaultDir).toString();
        QString outputTitle = QString::fromStdWString(m_item->title());

        QDir d;
        if (!d.exists(outputDir))
            if (!d.mkpath(outputDir))
                outputDir = defaultDir;


        mutex.lock();

        try
        {
            m_resultFileName = FileSystem::generateUniqueFilePath(outputDir, outputTitle, ".mp3");
            QFile file(m_resultFileName);
            if (file.open(QIODevice::WriteOnly))
                file.close();
        }
        catch(...)
        {
        }

        mutex.unlock();


        QFileInfo info(m_resultFileName);
        m_downloadFileName = info.dir().absoluteFilePath(FileSystem::generateFileName());

        m_downloader.reset(new downloader::media_downloader(m_item->media(index).get_media_url_handle(), m_item->media(index).get_media_info_handle(),
                                                            m_downloadFileName.toStdWString(), DownloadStateFunctor(this), DownloadProgressFunctor(this)));

        m_progressMetrics.reset(new ComponentModel::Progress(0, m_size, 2000, 100000));
        QObject::connect(m_progressMetrics.data(), SIGNAL(remainingTimeChanged(ComponentModel::PropertyChangedSignalArgs)),
                         this, SLOT(onRemainingTimeChanged(ComponentModel::PropertyChangedSignalArgs)));

        setState(DownloadItemViewModel::InitializedState);
        break;
    }

    default:
        setState(DownloadItemViewModel::ErrorState);
        break;
    }
}


void DownloadItemViewModel::onBasicAction()
{
    switch (m_state)
    {
    case DownloadState:
    case ConvertState:
        pause();
        break;

    case PauseState:
        resume();
        break;

    case DoneState:
    case PlayerPlayState:
    case PlayerPauseState:
        showInFolder();
        break;

    default:
        break;
    }
}


void DownloadItemViewModel::onRemainingTimeChanged(const ComponentModel::PropertyChangedSignalArgs& args)
{
    Q_UNUSED(args)
    setRemainingTime(m_progressMetrics->remainingTime());
}


void DownloadItemViewModel::onPlayerTimeChanged(qint64 playerTime)
{
    emitPropertyChanged("playingTime", playerTime);
}


void DownloadItemViewModel::onPlayerStateChanged(Multimedia::Player::State playerState)
{
    switch (playerState)
    {
    case Player::PlayState:
        setState(DownloadItemViewModel::PlayerPlayState);
        break;

    case Player::PauseState:
        setState(DownloadItemViewModel::PlayerPauseState);
        break;

    default:
        setState(DownloadItemViewModel::DoneState);
        break;
    }
}


void DownloadItemViewModel::onPlayerCompleted()
{
    emit playerCompleted();
}


bool DownloadItemViewModel::releaseLeasableObject(ObjectLessor* lessor)
{
    if (!m_playerLessor.isNull() && m_playerLessor.data() == lessor)
    {
        releasePlayer(false);
        return true;
    }

    return false;
}


bool DownloadItemViewModel::event(QEvent* e)
{
    switch (e->type())
    {
    case ParseResultEventType:
        {
            ParseResultEvent* pe = (ParseResultEvent*)e;
            doParsed(pe->result(), pe->error());
        }
        return true;


    case InitializeResultEventType:
        {
            InitializeResultEvent* ie = (InitializeResultEvent*)e;
            doInitialized(ie->result());
        }
        return true;


    case DownloadStateEventType:
        {
            DownloadStateEvent* de = (DownloadStateEvent*)e;

            switch (de->state())
            {
            case downloader::media_downloader::stateStart:
            case downloader::media_downloader::stateDownload:
            case downloader::media_downloader::stateResume:
                setState(DownloadItemViewModel::DownloadState);
                break;

            case downloader::media_downloader::statePause:
                setState(DownloadItemViewModel::PauseState);
                break;

            case downloader::media_downloader::stateFinish:
                audio::video2mp3::convert(
                std::string(m_downloadFileName.toUtf8().constData()),
                std::string(m_resultFileName.toUtf8().constData()),                
                title().toStdWString(), ConvertStateFunctor(this), false);
                setState(DownloadItemViewModel::ConvertState);
                break;

            default:
                setState(DownloadItemViewModel::ErrorState);
                break;
            }
        }
        return true;


    case DownloadProgressEventType:
        {
            if (m_size > 0)
            {
                DownloadProgressEvent* de = (DownloadProgressEvent*)e;
                setProgress(de->progress() * 100 / m_size);

                if (m_progressMetrics)
                    m_progressMetrics->setValue(de->progress());
            }
        }
        return true;


    case ConvertStateEventType:
        {
            ConvertStateEvent* ce = (ConvertStateEvent*)e;

            switch (ce->state())
            {
            case audio::video2mp3::stateProgress:
                setProgress(ce->progress() * 100);
                break;

            case audio::video2mp3::stateFinish:
                setState(DownloadItemViewModel::DoneState);

                emit downloadCompleted();

                QFile::remove(m_downloadFileName);
                m_downloadFileName.clear();
                break;

            default:
                setState(DownloadItemViewModel::ErrorState);
                break;
            }
        }
        return true;


    default:
        return false;
    }
}

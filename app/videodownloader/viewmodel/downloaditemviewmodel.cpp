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
#include "gui/cxx/thumbnail.h"
#include "viewmodel/mediadownload.h"
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
using namespace Mvvm;
using namespace Gui;
using namespace openmedia;


namespace
{

const QEvent::Type DownloadStateEventType = QEvent::Type(QEvent::User + 1);
const QEvent::Type DownloadProgressEventType = QEvent::Type(QEvent::User + 2);
const QEvent::Type ConvertStateEventType = QEvent::Type(QEvent::User + 3);

QMutex mutex;


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


DownloadItemViewModel::DownloadItemViewModel(const Mvvm::Dialog* dialog, QObject* parent) :
    NotifyObject(parent),
    m_dialog(dialog),
    m_downloadFileName(""),
    m_resultFileName(""),
    m_url(""),
    m_size(0),
    m_duration(0),
    m_format(""),
    m_media(DownloadItemViewModel::Video),
    m_progress(-1),
    m_remainingTime(0),
    m_state(DownloadItemViewModel::None),
    m_basicAction(this),
    m_folderAction(this),
    m_urlAction(this),
    m_facebookAction(this),
    m_twitterAction(this)
{
    qRegisterMetaType<Media>();
    qRegisterMetaType<State>();

    m_basicAction.setEnabled(false);
    m_basicAction.setVisible(false);
    m_basicAction.setText("");
    m_basicAction.setToolTip("");
    QObject::connect(&m_basicAction, SIGNAL(triggered()), this, SLOT(doBasicAction()));

    m_folderAction.setEnabled(false);
    m_folderAction.setText(tr("Show in folder"));
    m_folderAction.setToolTip("Open folder with downloaded file");
    QObject::connect(&m_folderAction, SIGNAL(triggered()), this, SLOT(showInFolder()));

    m_urlAction.setEnabled(true);
    m_urlAction.setText(tr("Copy url"));
    m_urlAction.setToolTip("Copty url address");
    QObject::connect(&m_urlAction, SIGNAL(triggered()), this, SLOT(copyUrl()));

    m_facebookAction.setEnabled(true);
    m_facebookAction.setText(tr("Share on Facebook"));
    m_facebookAction.setToolTip("Share link to clip on facebook");
    m_facebookAction.setIcon(QIcon(":/image/item-facebook"));
    QObject::connect(&m_facebookAction, SIGNAL(triggered()), this, SLOT(shareOnFacebook()));

    m_twitterAction.setEnabled(true);
    m_twitterAction.setText(tr("Share on Twitter"));
    m_twitterAction.setToolTip("Share link to clip on twitter");
    m_twitterAction.setIcon(QIcon(":/image/item-twitter"));
    QObject::connect(&m_twitterAction, SIGNAL(triggered()), this, SLOT(shareOnTwitter()));
}


DownloadItemViewModel::DownloadItemViewModel(const Mvvm::Dialog* dialog, ViewModel::MediaDownloadItem item, QObject* parent) :
    NotifyObject(parent),
    m_dialog(dialog),
    m_url(""),
    m_size(0),
    m_duration(0),
    m_format(""),
    m_media(DownloadItemViewModel::Video),
    m_progress(-1),
    m_remainingTime(0),
    m_state(DownloadItemViewModel::None),
    m_basicAction(this),
    m_folderAction(this),
    m_urlAction(this),
    m_facebookAction(this),
    m_twitterAction(this)
{
    qRegisterMetaType<Media>();
    qRegisterMetaType<State>();

    m_basicAction.setEnabled(false);
    m_basicAction.setVisible(false);
    m_basicAction.setText("");
    m_basicAction.setToolTip("");
    QObject::connect(&m_basicAction, SIGNAL(triggered()), this, SLOT(doBasicAction()));

    m_folderAction.setEnabled(false);
    m_folderAction.setText(tr("Show in folder"));
    m_folderAction.setToolTip("Open folder with downloaded file");
    QObject::connect(&m_folderAction, SIGNAL(triggered()), this, SLOT(showInFolder()));

    m_urlAction.setEnabled(true);
    m_urlAction.setText(tr("Copy url"));
    m_urlAction.setToolTip("Copy url address");
    QObject::connect(&m_urlAction, SIGNAL(triggered()), this, SLOT(copyUrl()));

    m_facebookAction.setEnabled(true);
    m_facebookAction.setText(tr("Facebook"));
    m_facebookAction.setToolTip("Share link to video on facebook");
    QObject::connect(&m_facebookAction, SIGNAL(triggered()), this, SLOT(shareOnFacebook()));

    m_twitterAction.setEnabled(true);
    m_twitterAction.setText(tr("Twitter"));
    m_twitterAction.setToolTip("Share link to video on twitter");
    QObject::connect(&m_twitterAction, SIGNAL(triggered()), this, SLOT(shareOnTwitter()));

    initialize(item);
}


DownloadItemViewModel::DownloadItemViewModel(const Mvvm::Dialog* dialog, ViewModel::DownloadSettingsViewModel* settings, QObject* parent) :
    NotifyObject(parent),
    m_dialog(dialog),
    m_settings(settings),
    m_url(""),
    m_size(0),
    m_duration(0),
    m_format(""),
    m_media(DownloadItemViewModel::Video),
    m_progress(-1),
    m_remainingTime(0),
    m_state(DownloadItemViewModel::Parse),
    m_basicAction(this),
    m_folderAction(this),
    m_urlAction(this),
    m_facebookAction(this),
    m_twitterAction(this)
{
    qRegisterMetaType<Media>();
    qRegisterMetaType<State>();

    m_basicAction.setEnabled(false);
    m_basicAction.setVisible(false);
    m_basicAction.setText("");
    m_basicAction.setToolTip("");
    QObject::connect(&m_basicAction, SIGNAL(triggered()), this, SLOT(doBasicAction()));

    m_folderAction.setEnabled(false);
    m_folderAction.setText(tr("Show in folder"));
    m_folderAction.setToolTip("Open folder with downloaded file");
    QObject::connect(&m_folderAction, SIGNAL(triggered()), this, SLOT(showInFolder()));

    m_urlAction.setEnabled(true);
    m_urlAction.setText(tr("Copy url"));
    m_urlAction.setToolTip("Copy url address");
    QObject::connect(&m_urlAction, SIGNAL(triggered()), this, SLOT(copyUrl()));

    m_facebookAction.setEnabled(true);
    m_facebookAction.setText(tr("Facebook"));
    m_facebookAction.setToolTip("Share link to video on facebook");
    QObject::connect(&m_facebookAction, SIGNAL(triggered()), this, SLOT(shareOnFacebook()));

    m_twitterAction.setEnabled(true);
    m_twitterAction.setText(tr("Twitter"));
    m_twitterAction.setToolTip("Share link to video on twitter");
    QObject::connect(&m_twitterAction, SIGNAL(triggered()), this, SLOT(shareOnTwitter()));

    setThumbnail(QPixmap(":/image/thumbnail-default"));

    QObject::connect(m_settings.data(), SIGNAL(propertyChanged(ComponentModel::PropertyChangedSignalArgs)),
                     this, SLOT(doSettingsPropertyChanged(ComponentModel::PropertyChangedSignalArgs)));
}


DownloadItemViewModel::~DownloadItemViewModel()
{
    if (m_media == DownloadItemViewModel::Audio && !m_downloadFileName.isEmpty())
    {
        QFile::remove(m_downloadFileName);
    }
}


void DownloadItemViewModel::initialize(ViewModel::MediaDownloadItem item)
{
    m_url = QString::fromStdString(item.mediaList()->url());

    try
    {
        if (item.mediaList()->thumbnail().get())
        {
            QImage image = QImage::fromData(&(item.mediaList()->thumbnail()->at(0)), item.mediaList()->thumbnail()->size(), "jpg");
            setThumbnail(QPixmap::fromImage(image));
        }
        else
        {
            setThumbnail(QPixmap(":/image/thumbnail-default"));
        }
    }
    catch(...)
    {
        setThumbnail(QPixmap(":/image/thumbnail-default"));
    }


    mutex.lock();

    try
    {
        m_resultFileName = item.mediaFileName();

        QFile file(m_resultFileName);
        if (file.open(QIODevice::WriteOnly))
            file.close();
    }
    catch(...)
    {
    }

    mutex.unlock();

    m_downloadFileName = m_resultFileName;

    if (item.downloadType() == ViewModel::ExtractAudio)
    {
        QFileInfo info(m_resultFileName);
        QString filename = FileSystem::generateFileName();
        m_downloadFileName = info.dir().absoluteFilePath(filename);
    }    

    m_size = qint64(item.media().content_size());
    m_duration = qint32(item.mediaList()->duration());
    m_format = MediaDownloadDetails::format(item.downloadType(), item.mediaList(), item.mediaIndex());
    m_media = item.downloadType() == ViewModel::DownloadVideo ? Video : Audio;

    m_downloader.reset(new downloader::media_downloader(item.media().get_media_url_handle(), item.media().get_media_info_handle(),
                                                        m_downloadFileName.toStdWString(), DownloadStateFunctor(this), DownloadProgressFunctor(this)));

    m_progressMetrics.reset(new ComponentModel::Progress(0, m_size, 2000, 100000));
    QObject::connect(m_progressMetrics.data(), SIGNAL(remainingTimeChanged(ComponentModel::PropertyChangedSignalArgs)),
                     this, SLOT(doRemainingTimeChanged(ComponentModel::PropertyChangedSignalArgs)));

    if (item.subtitlesIndex() != -1)
        item.mediaList()->subtitle_save(item.subtitlesIndex(), std::string(item.subtitlesFileName().toUtf8().data()));


    emitPropertyChanged("title", QVariant::fromValue(title()));
    emitPropertyChanged("size", QVariant::fromValue(size()));
    emitPropertyChanged("duration", QVariant::fromValue(duration()));
    emitPropertyChanged("format", QVariant::fromValue(format()));
    emitPropertyChanged("media", QVariant::fromValue(media()));
}


bool DownloadItemViewModel::serialize(QDomDocument& doc, QDomElement& element) const
{
    element = doc.createElement("Item");
    element.setAttribute("filename", m_resultFileName);
    element.setAttribute("url", m_url);
    element.setAttribute("duration", (int)m_duration);
    element.setAttribute("format", m_format);
    element.setAttribute("media", (int)m_media);
    element.setAttribute("thumbnail", Transform::pixmapToBase64(m_thumbnail));

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
    m_format = element.attribute("format");
    m_size = info.size();
    m_media = (DownloadItemViewModel::Media)element.attribute("media").toInt();

    setThumbnail(Transform::base64ToPixmap(element.attribute("thumbnail")));

    setState(DownloadItemViewModel::Done);

    return true;
}


QString DownloadItemViewModel::title() const
{
    if (m_state == Parse)
        return tr("Retrieving information...");

    QFileInfo info(m_resultFileName);
    return info.completeBaseName();
}


QPixmap DownloadItemViewModel::thumbnail() const
{
    return m_thumbnail;
}


void DownloadItemViewModel::setThumbnail(QPixmap value)
{
    Thumbnail::apply(value, m_thumbnail);
    emitPropertyChanged("thumbnail");
}


QString DownloadItemViewModel::size() const
{
    return Transform::storageSizeToString(m_size);
}


QString DownloadItemViewModel::duration() const
{
    if (m_duration == 0)
        return QString();

    return Transform::timeToString(m_duration, "H:mm:ss");
}


QString DownloadItemViewModel::format() const
{
    return m_format;
}


DownloadItemViewModel::Media DownloadItemViewModel::media() const
{
    return m_media;
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
    if ((m_state == Download || m_state == Pause) && m_remainingTime > 0)
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
            if (m_state == Download)
                m_progressMetrics->start();
            else
                m_progressMetrics->stop();
        }


        switch (m_state)
        {
        case Parse:
            m_basicAction.setEnabled(false);
            m_basicAction.setVisible(false);
            break;

        case Download:
            m_basicAction.setEnabled(true);
            m_basicAction.setVisible(true);
            m_basicAction.setText(tr("Pause"));
            m_basicAction.setToolTip(tr("Pause download"));
            break;

        case Convert:
            m_basicAction.setEnabled(true);
            m_basicAction.setVisible(true);
            m_basicAction.setText(tr("Pause"));
            m_basicAction.setToolTip(tr("Pause convertion"));
            break;

        case Pause:
            m_basicAction.setEnabled(true);
            m_basicAction.setVisible(true);
            m_basicAction.setText(tr("Resume"));
            m_basicAction.setToolTip(tr("Resume"));
            break;

        case Done:
        {
            m_basicAction.setEnabled(true);
            m_basicAction.setVisible(true);
            m_basicAction.setText(tr("Play"));
            m_basicAction.setToolTip(tr("Play downloaded file"));

            m_folderAction.setEnabled(true);

            QFileInfo info(m_resultFileName);
            m_size = info.size();
            emitPropertyChanged("size", size());

            m_progressMetrics.reset();

            break;
        }

        case Error:
            m_basicAction.setEnabled(false);
            m_basicAction.setVisible(false);

            m_progressMetrics.reset();
            break;

        default:
            break;
        }

        emitPropertyChanged("state", m_state);
        emitPropertyChanged("remainingTime", remainingTime());
    }
}


QAction* DownloadItemViewModel::basicAction()
{
    return &m_basicAction;
}


QAction* DownloadItemViewModel::folderAction()
{
    return &m_folderAction;
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


void DownloadItemViewModel::doBasicAction()
{
    switch (m_state)
    {
    case Download:
    case Convert:
        pause();
        break;

    case Pause:
        resume();
        break;

    case Done:
        play();
        break;

    default:
        break;
    }
}


void DownloadItemViewModel::doRemainingTimeChanged(const ComponentModel::PropertyChangedSignalArgs& args)
{
    Q_UNUSED(args)
    setRemainingTime(m_progressMetrics->remainingTime());
}


void DownloadItemViewModel::doSettingsPropertyChanged(const ComponentModel::PropertyChangedSignalArgs& args)
{
    if (args.property() == "state" && !m_settings.isNull())
    {
         if (m_settings->state() == DownloadSettingsViewModel::Done)
         {
             MediaDownloadList downloadList = m_settings->downloadList();

             if (downloadList.count() > 0)
             {
                 setState(None);
                 initialize(downloadList.at(0));
                 emit parsed(downloadList);
             }
             else
             {
                 setState(Error);
             }

             m_settings.clear();
         }
         else if (m_settings->state() == DownloadSettingsViewModel::Error)
         {
             setState(Error);
             m_settings.clear();
         }
    }
}


void DownloadItemViewModel::pause()
{
    if (m_state != Download)
        return;

    m_downloader->pause();
}


void DownloadItemViewModel::resume()
{
    if (m_state != Pause)
        return;

    m_downloader->resume();
}


void DownloadItemViewModel::showInFolder()
{
    if (m_state != Done)
        return;

    FileSystem::showFile(m_resultFileName);
}


void DownloadItemViewModel::play()
{
    if (m_state != Done)
        return;

    FileSystem::openFile(m_resultFileName);
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


bool DownloadItemViewModel::event(QEvent* e)
{
    switch (e->type())
    {
    case DownloadStateEventType:
        {
            DownloadStateEvent* de = (DownloadStateEvent*)e;

            switch (de->state())
            {
            case downloader::media_downloader::stateStart:
            case downloader::media_downloader::stateDownload:
            case downloader::media_downloader::stateResume:
                setState(DownloadItemViewModel::Download);
                break;

            case downloader::media_downloader::statePause:
                setState(DownloadItemViewModel::Pause);
                break;

            case downloader::media_downloader::stateFinish:
                {
                    if (m_media == DownloadItemViewModel::Audio)
                    {
                        audio::video2mp3::convert(
                        std::string(m_downloadFileName.toUtf8().constData()),
                        std::string(m_resultFileName.toUtf8().constData()),
                        title().toStdWString(),
                        ConvertStateFunctor(this),
                        false);
                        
                        setState(DownloadItemViewModel::Convert);
                    }
                    else
                    {
                        setState(DownloadItemViewModel::Done);
                        emit downloadCompleted();
                    }
                }
                break;

            default:
                setState(DownloadItemViewModel::Error);
                break;
            }
        }
        return true;


    case DownloadProgressEventType:
        {
            DownloadProgressEvent* de = (DownloadProgressEvent*)e;
            setProgress(de->progress() * 100 / m_size);

            if (m_progressMetrics)
                m_progressMetrics->setValue(de->progress());
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
                setState(DownloadItemViewModel::Done);

                emit downloadCompleted();

                QFile::remove(m_downloadFileName);
                m_downloadFileName.clear();
                break;

            default:
                setState(DownloadItemViewModel::Error);
                break;
            }
        }
        return true;


    default:
        return false;
    }
}

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
#include "social/facebook.h"
#include "social/twitter.h"
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
#include <Shellapi.h>
#endif

using namespace ViewModel;
using namespace ComponentModel;
using namespace Mvvm;
using namespace Social;
using namespace openmedia;


const QEvent::Type DownloadStateEventType = QEvent::Type(QEvent::User + 1);
const QEvent::Type DownloadProgressEventType = QEvent::Type(QEvent::User + 2);
const QEvent::Type ConvertStateEventType = QEvent::Type(QEvent::User + 3);
const QEvent::Type ParseResultEventType = QEvent::Type(QEvent::User + 4);
const QEvent::Type InitializeResultEventType = QEvent::Type(QEvent::User + 5);

namespace
{
    QMutex mutex;
}


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


DownloadItemViewModel::DownloadItemViewModel(const Mvvm::Dialog* dialog, QObject* parent) :
    NotifyObject(parent),
    m_dialog(dialog),
    m_downloadFileName(""),
    m_resultFileName(""),
    m_url(""),
    m_size(0),
    m_duration(0),
    m_progress(-1),
    m_remainingTime(0),
    m_state(DownloadItemViewModel::None),
    m_basicAction(this),
    m_folderAction(this),
    m_urlAction(this),
    m_facebookAction(this),
    m_twitterAction(this)
{
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


DownloadItemViewModel::DownloadItemViewModel(const Mvvm::Dialog* dialog, QString url, bool isPlaylist, QObject* parent) :
    NotifyObject(parent),
    m_dialog(dialog),
    m_downloadFileName(""),
    m_resultFileName(""),
    m_url(url),
    m_size(0),
    m_duration(0),
    m_progress(-1),
    m_remainingTime(0),
    m_state(DownloadItemViewModel::None),
    m_basicAction(this),
    m_folderAction(this),
    m_urlAction(this),
    m_facebookAction(this),
    m_twitterAction(this)
{
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
    m_facebookAction.setText(tr("Share on Facebook"));
    m_facebookAction.setToolTip("Share link to clip on facebook");
    m_facebookAction.setIcon(QIcon(":/image/item-facebook"));
    QObject::connect(&m_facebookAction, SIGNAL(triggered()), this, SLOT(shareOnFacebook()));

    m_twitterAction.setEnabled(true);
    m_twitterAction.setText(tr("Share on Twitter"));
    m_twitterAction.setToolTip("Share link to clip on twitter");
    m_twitterAction.setIcon(QIcon(":/image/item-twitter"));
    QObject::connect(&m_twitterAction, SIGNAL(triggered()), this, SLOT(shareOnTwitter()));

    setState(DownloadItemViewModel::Parse);

    downloader::url_parser::parse_url_async(url.toStdString(),
                                            isPlaylist ? downloader::url_parser::parsePlaylist : downloader::url_parser::parseNormal,
                                            ParseResultFunctor(this));
}


DownloadItemViewModel::DownloadItemViewModel(const Mvvm::Dialog* dialog, openmedia::downloader::media_download_list_ptr item, QObject* parent) :
    NotifyObject(parent),
    m_dialog(dialog),
    m_downloadFileName(""),
    m_resultFileName(""),
    m_url(""),
    m_size(0),
    m_duration(0),
    m_item(item),
    m_progress(-1),
    m_remainingTime(0),
    m_state(DownloadItemViewModel::None),
    m_basicAction(this),
    m_folderAction(this),
    m_urlAction(this),
    m_facebookAction(this),
    m_twitterAction(this)
{
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

    m_url = QString::fromStdString(item->url());

    setState(DownloadItemViewModel::Parse);

    m_item->initialize(InitializeResultFunctor(this));
}


DownloadItemViewModel::~DownloadItemViewModel()
{
    if (!m_downloadFileName.isEmpty())
    {
        QFile::remove(m_downloadFileName);
    }
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

    setState(DownloadItemViewModel::Done);

    return true;
}


QString DownloadItemViewModel::title() const
{
    switch (state())
    {
        case DownloadItemViewModel::Parse:
            return tr("Retrieving information...");

        case DownloadItemViewModel::Error:
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
        case Initialized:
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
        emitPropertyChanged("title", title());
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

    QFileInfo info = QFileInfo(m_resultFileName);

#if defined(Q_OS_WIN)
    QString url = info.canonicalFilePath();
    url.replace("/", "\\");
    url = "/select, \"" + url + "\"";
    ShellExecute(NULL, L"open", L"explorer.exe", (WCHAR*)url.utf16(), NULL, 5);
#else
    QString url = info.canonicalPath();
    QDesktopServices::openUrl(QUrl("file:///"+url));
#endif
}


void DownloadItemViewModel::play()
{
    if (m_state != Done)
        return;

    QFileInfo info = QFileInfo(m_resultFileName);
    QString url = info.canonicalFilePath();
    QDesktopServices::openUrl(QUrl("file:///"+url));
}


void DownloadItemViewModel::copyUrl()
{
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(m_url);
}


void DownloadItemViewModel::shareOnFacebook()
{
    QSettings settings;
    QString socialUrl = settings.value("socialUrl").toString();

    Facebook::shareLink(m_url, tr("I like this video! Download it with %1").arg(socialUrl));
}


void DownloadItemViewModel::shareOnTwitter()
{
    QSettings settings;
    QString socialReferrer = settings.value("socialReferrer").toString();
    QString socialVia = settings.value("socialVia").toString();

    Twitter::shareLink(m_url, tr("I like this video!"), socialReferrer, socialVia);
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
        setState(DownloadItemViewModel::Error);
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
            setState(DownloadItemViewModel::Error);
            break;
        }

        m_size = qint64(m_item->media(index).content_size());
        m_duration = qint32(m_item->duration());

        QSettings settings;
        QString outputDir = QDesktopServices::storageLocation(QDesktopServices::MusicLocation);
        outputDir = settings.value("Download/outputDirName", outputDir).toString();
        QString outputTitle = QString::fromStdWString(m_item->title());


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
                         this, SLOT(doRemainingTimeChanged(ComponentModel::PropertyChangedSignalArgs)));

        setState(DownloadItemViewModel::Initialized);
        break;
    }

    default:
        setState(DownloadItemViewModel::Error);
        break;
    }
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
                setState(DownloadItemViewModel::Download);
                break;

            case downloader::media_downloader::statePause:
                setState(DownloadItemViewModel::Pause);
                break;

            case downloader::media_downloader::stateFinish:
                audio::video2mp3::convert(m_downloadFileName.toStdWString(), m_resultFileName.toStdWString(), title().toStdWString(), ConvertStateFunctor(this));
                setState(DownloadItemViewModel::Convert);
                break;

            default:
                setState(DownloadItemViewModel::Error);
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
                setState(DownloadItemViewModel::Done);

                QFile::remove(m_downloadFileName);
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

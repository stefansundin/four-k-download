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


#include "viewmodel/downloadsettingsviewmodel.h"
#include "componentmodel/transform.h"
#include "componentmodel/filesystem.h"
#include "gui/cxx/thumbnail.h"
#include <QCoreApplication>
#include <QPointer>
#include <QSettings>
#include <QDesktopServices>

using namespace ViewModel;
using namespace Mvvm;
using namespace ComponentModel;
using namespace Gui;
using namespace openmedia;


namespace
{

const QEvent::Type ParseResultEventType = QEvent::Type(QEvent::User + 4);


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

} // Anonimous


DownloadSettingsViewModel::DownloadSettingsViewModel(const Mvvm::Dialog* dialog, QString url, bool isPlaylist, QObject* parent) :
    ComponentModel::NotifyObject(parent),
    m_dialog(dialog),
    m_detailsIndex(-1),
    m_subtitlesIndex(-1),
    m_subtitlesDownload(false),
    m_smartMode(false),
    m_outputDirName(""),
    m_outputFileName(""),
    m_outputPathAction(this),
    m_checkAllAction(this),
    m_uncheckAllAction(this),
    m_state(DownloadSettingsViewModel::Parsing)
{
    qRegisterMetaType<DownloadSettingsViewModel::State>();

    m_outputPathAction.setEnabled(false);
    m_outputPathAction.setText(tr("..."));
    m_outputPathAction.setToolTip(tr("Change output file path"));
    QObject::connect(&m_outputPathAction, SIGNAL(triggered()), this, SLOT(browse()));

    m_checkAllAction.setEnabled(false);
    m_checkAllAction.setText(tr("Check All"));
    m_checkAllAction.setToolTip(tr("Check all items"));
    QObject::connect(&m_checkAllAction, SIGNAL(triggered()), this, SLOT(checkAll()));

    m_uncheckAllAction.setEnabled(false);
    m_uncheckAllAction.setText(tr("Uncheck All"));
    m_uncheckAllAction.setToolTip(tr("Uncheck all items"));
    QObject::connect(&m_uncheckAllAction, SIGNAL(triggered()), this, SLOT(uncheckAll()));

    initialize();

    downloader::url_parser::parse_url_async(url.toStdString(),
                                            isPlaylist ? downloader::url_parser::parsePlaylist : downloader::url_parser::parseNormal,
                                            ParseResultFunctor(this));
}


DownloadSettingsViewModel::~DownloadSettingsViewModel()
{
    deinitialize();
}


void DownloadSettingsViewModel::initialize()
{
    QSettings settings;
    QString path = QDesktopServices::storageLocation(QDesktopServices::MoviesLocation);
    m_outputDirName = settings.value("Download/outputDirName", path).toString();
    m_smartMode = settings.value("Download/smartMode", false).toBool();

    QDir d;
    if (!d.exists(m_outputDirName))
        m_outputDirName = path;
}


void DownloadSettingsViewModel::deinitialize()
{
    QSettings settings;
    if (!m_outputDirName.isEmpty())
        settings.setValue("Download/outputDirName", m_outputDirName);

    saveDefaults();
}


void DownloadSettingsViewModel::saveDefaults()
{
    QSettings settings;

    if (detailsIndex() != -1 && !m_smartMode)
    {
        settings.setValue("Download/defaultMediaQuality", (int)m_detailsList[detailsIndex()]->mediaQuality());
        settings.setValue("Download/defaultMediaContent", (int)m_detailsList[detailsIndex()]->mediaContent());
        settings.setValue("Download/defaultDownloadType", (int)m_detailsList[detailsIndex()]->downloadType());
    }

    if (isSubtitlesEnabled())
    {
        settings.setValue("Download/defaultSubtitlesDownload", isSubtitlesDownload());

        if (isSubtitlesDownload() && subtitlesIndex() != -1)
            settings.setValue("Download/defaultSubtitles", m_subtitlesList[subtitlesIndex()]);
    }
}


void DownloadSettingsViewModel::restoreDefaults()
{
    QSettings settings;

    downloader::media_quality_type_t quality = (downloader::media_quality_type_t)settings.value("Download/defaultMediaQuality", (int)downloader::mediaQuality480P).toInt();
    downloader::media_content_type_t content = (downloader::media_content_type_t)settings.value("Download/defaultMediaContent", (int)downloader::mediaContentVideoFlv).toInt();
    MediaDownloadType type = (MediaDownloadType)settings.value("Download/defaultDownloadType", (int)DownloadVideo).toInt();
    int index = 0;
    int similarity = -1;

    for (int i = 0; i < m_detailsList.count(); ++i)
    {
        if (type == m_detailsList[i]->downloadType())
        {
            int s = downloader::get_media_similarity(
                        quality, content,
                        m_detailsList[i]->mediaQuality(),
                        m_detailsList[i]->mediaContent());
            if (similarity < s)
            {
                similarity = s;
                index = i;
            }
        }
    }

    setDetailsIndex(index);


    if (m_mediaList.count() == 1) // Clip
    {
        QString subtitles = settings.value("Download/defaultSubtitles", "English").toString();
        for (int i = 0; i < m_subtitlesList.count(); ++i)
        {
            if (subtitles == m_subtitlesList[i])
            {
                setSubtitlesIndex(i);
                setSubtitlesDownload(settings.value("Download/defaultSubtitlesDownload", false).toBool());
                break;
            }
        }

        if (subtitlesIndex() == -1)
            setSubtitlesIndex(0);
    }
}


MediaDownloadList DownloadSettingsViewModel::downloadList() const
{
    MediaDownloadList result;

    if (detailsIndex() != -1)
    {
        QSharedPointer<MediaDetailsViewModel> details = m_detailsList[detailsIndex()];

        int stIndex = -1;
        if (isSubtitlesEnabled() && isSubtitlesDownload())
            stIndex = subtitlesIndex();

        for (int i = 0; i < m_mediaList.count(); ++i)
        {
            if (!m_mediaList[i]->isChecked() || m_mediaList[i]->state() != MediaItemViewModel::Done)
                continue;

            int mediaIndex = m_sdkResult->at(i)->find(details->mediaQuality(), details->mediaContent());
            if (mediaIndex != -1)
                result.append(MediaDownloadItem(details->downloadType(), m_sdkResult->at(i), mediaIndex, stIndex, outputPath(),
                                                m_mediaList.count() > 1 || m_smartMode /*generate if playlist ot smart mode*/));
        }
    }

    return result;
}


QList< QSharedPointer<MediaItemViewModel> > DownloadSettingsViewModel::mediaList() const
{
    return m_mediaList;
}


QList< QSharedPointer<MediaDetailsViewModel> > DownloadSettingsViewModel::detailsList() const
{
    return m_detailsList;
}


int DownloadSettingsViewModel::detailsIndex() const
{
    return m_detailsIndex;
}


void DownloadSettingsViewModel::setDetailsIndex(int value)
{
    if (m_detailsIndex != value)
    {
        if (value >= m_detailsList.count())
            value = m_detailsList.count() - 1;

        m_detailsIndex = value;

        emitPropertyChanged("detailsIndex", detailsIndex());
        emitPropertyChanged("subtitlesEnabled", isSubtitlesEnabled());
        emitPropertyChanged("outputPath", outputPath());
    }
}


QStringList DownloadSettingsViewModel::subtitlesList() const
{
    return m_subtitlesList;
}


int DownloadSettingsViewModel::subtitlesIndex() const
{
    return m_subtitlesIndex;
}


void DownloadSettingsViewModel::setSubtitlesIndex(int value)
{
    if (m_subtitlesIndex != value)
    {
        if (value >= m_subtitlesList.count())
            value = m_subtitlesList.count() - 1;

        m_subtitlesIndex = value;

        emitPropertyChanged("subtitlesIndex", subtitlesIndex());
    }
}


bool DownloadSettingsViewModel::isSubtitlesDownload() const
{
    return m_subtitlesDownload;
}


void DownloadSettingsViewModel::setSubtitlesDownload(bool value)
{
    if (m_subtitlesDownload != value)
    {
        m_subtitlesDownload = value;

        emitPropertyChanged("subtitlesDownload", isSubtitlesDownload());
    }
}


bool DownloadSettingsViewModel::isSubtitlesEnabled() const
{
    return  !m_smartMode && m_mediaList.count() == 1 && m_subtitlesList.count() > 0 && detailsIndex() != -1 &&
            m_detailsList[detailsIndex()]->downloadType() == ViewModel::DownloadVideo;
}


QString DownloadSettingsViewModel::outputPath() const
{
    if (m_mediaList.count() == 1 && !m_smartMode)
    {
        // Clip

        if (state() != Done || detailsIndex() == -1 )
            return "";

        QString ext = m_detailsList[detailsIndex()]->fileExt();
        QDir dir(m_outputDirName);
        return dir.absoluteFilePath(m_outputFileName + ext);
    }
    else
    {
        // Playlist
        return m_outputDirName;
    }
}


void DownloadSettingsViewModel::setOutputPath(QString value)
{
    if (value.isEmpty())
        return;

    if (m_mediaList.count() == 1 && !m_smartMode)
    {
        // Clip
        QFileInfo info(value);

        m_outputDirName = info.absoluteDir().absolutePath();
        m_outputFileName = info.completeBaseName();
    }
    else
    {
        // Playlist
        m_outputDirName = value;
    }

    emitPropertyChanged("outputPath", outputPath());
}


QAction* DownloadSettingsViewModel::outputPathAction()
{
    return &m_outputPathAction;
}


QAction* DownloadSettingsViewModel::checkAllAction()
{
    return &m_checkAllAction;
}


QAction* DownloadSettingsViewModel::uncheckAllAction()
{
    return &m_uncheckAllAction;
}


DownloadSettingsViewModel::State DownloadSettingsViewModel::state() const
{
    return m_state;
}


bool DownloadSettingsViewModel::canClose()
{
    QString defaultDir = QDesktopServices::storageLocation(QDesktopServices::MoviesLocation);

    QDir d;
    if (!d.exists(m_outputDirName))
        if (!d.mkpath(m_outputDirName))
            m_outputDirName = defaultDir;


    QFileInfo info(outputPath());

    if (info.exists() && info.isFile())
    {
        QList<Mvvm::Dialog::MessageButton> buttons;
        buttons << Mvvm::Dialog::MessageButton(tr("Overwrite"), QMessageBox::AcceptRole);
        buttons << Mvvm::Dialog::MessageButton(tr("Cancel"), QMessageBox::RejectRole);

        int result = m_dialog.data()->showMessageDialog(tr("File \"%1\" already exists. Are you sure want to overwrite it?").arg(info.fileName()),
                                                        QMessageBox::Question, QCoreApplication::applicationName(), buttons, 1);

        if (result == 1)
            return false;
    }

    return true;
}


void DownloadSettingsViewModel::parsed(downloader::url_parser_result_ptr result, downloader::url_parser::ErrorCode error)
{
    if (!result || result->size() == 0)
        error = downloader::url_parser::errorUnknown;

    switch (error)
    {
    case downloader::url_parser::errorNone:
    {
        m_state = Retreiving;
        m_sdkResult = result;

        // Add media

        for (unsigned int i = 0; i < m_sdkResult->size(); ++i)
        {
            MediaItemViewModel* vm = new MediaItemViewModel(m_dialog.data(), m_sdkResult->at(i));
            m_mediaList.append(QSharedPointer<MediaItemViewModel>(vm));            
            QObject::connect(vm, SIGNAL(propertyChanged(ComponentModel::PropertyChangedSignalArgs)),
                             this, SLOT(itemPropertyChanged(ComponentModel::PropertyChangedSignalArgs)));
        }

        m_checkAllAction.setEnabled(m_mediaList.count() > 1);
        m_uncheckAllAction.setEnabled(m_mediaList.count() > 1);

        emitPropertyChanged("mediaList");
        break;
    }

    default:
        m_state = Error;
        break;
    }

    emitPropertyChanged("state", QVariant::fromValue(state()));
}


void DownloadSettingsViewModel::retreived()
{
    m_state = Done;

    // Add details

    if (m_mediaList.count() == 1)
    {
        // Clip

        for (unsigned int i = 0; i < m_sdkResult->at(0)->size(); ++i)
        {
            MediaDetailsViewModel* vm = new MediaDetailsViewModel(m_dialog.data(), ViewModel::DownloadVideo, m_sdkResult->at(0), i);
            m_detailsList.append(QSharedPointer<MediaDetailsViewModel>(vm));
        }

        for (int i = m_sdkResult->at(0)->size() - 1; i >= 0 ; --i)
        {
            downloader::media_download_info info = m_sdkResult->at(0)->media(i);

            if (info.quality() >= downloader::mediaQualityOriginal &&
                info.quality() <= downloader::mediaQuality360P &&
                info.audio_codec() != downloader::mediaAudioMP3)
            {
                MediaDetailsViewModel* vm = new MediaDetailsViewModel(m_dialog.data(), ViewModel::ExtractAudio, m_sdkResult->at(0), i);
                m_detailsList.append(QSharedPointer<MediaDetailsViewModel>(vm));
                break;
            }
        }

        for (unsigned int i = 0; i < m_sdkResult->at(0)->subtitles_count(); ++i)
        {
            m_subtitlesList.append(QString::fromStdString(m_sdkResult->at(0)->subtitle_lang_at(i)));
        }
    }
    else
    {
        // Playlist

        std::vector< downloader::media_desc_type > qualities = m_sdkResult->qualities();
        for (unsigned int i = 0; i < qualities.size(); ++i)
        {
            downloader::media_desc_type item = qualities[i];
            MediaDetailsViewModel* vm = new MediaDetailsViewModel(m_dialog.data(), ViewModel::DownloadVideo, item.quality, item.format,
                                                                  MediaDownloadDetails::summary(ViewModel::DownloadVideo, item.quality),
                                                                  MediaDownloadDetails::quality(ViewModel::DownloadVideo, item.quality),
                                                                  MediaDownloadDetails::codecs(ViewModel::DownloadVideo, item.format, item.video, item.audio),
                                                                  MediaDownloadDetails::format(ViewModel::DownloadVideo, item.format),
                                                                  MediaDownloadDetails::fileExt(ViewModel::DownloadVideo, item.format),
                                                                  "");
            m_detailsList.append(QSharedPointer<MediaDetailsViewModel>(vm));
        }

        for (unsigned int i = 0; i < qualities.size(); ++i)
        {
            downloader::media_desc_type item = qualities[i];

            if (item.quality >= downloader::mediaQualityOriginal &&
                item.quality <= downloader::mediaQuality360P &&
                item.audio != downloader::mediaAudioMP3)
            {
                MediaDetailsViewModel* vm = new MediaDetailsViewModel(m_dialog.data(), ViewModel::ExtractAudio, item.quality, item.format,
                                                                      MediaDownloadDetails::summary(ViewModel::ExtractAudio, item.quality),
                                                                      MediaDownloadDetails::quality(ViewModel::ExtractAudio, item.quality),
                                                                      MediaDownloadDetails::codecs(ViewModel::ExtractAudio, item.format, item.video, item.audio),
                                                                      MediaDownloadDetails::format(ViewModel::ExtractAudio, item.format),
                                                                      MediaDownloadDetails::fileExt(ViewModel::ExtractAudio, item.format),
                                                                      "");
                m_detailsList.append(QSharedPointer<MediaDetailsViewModel>(vm));
                break;
            }
        }
    }

    m_outputFileName = (m_mediaList.count() == 1 && !m_smartMode) ?
                FileSystem::filterInvalidSymbols(QString::fromStdWString(m_sdkResult->at(0)->title())) :
                "";
    m_outputPathAction.setEnabled(true);

    emitPropertyChanged("detailsList");
    emitPropertyChanged("subtitlesList");

    restoreDefaults();

    emitPropertyChanged("state", QVariant::fromValue(state()));
}


void DownloadSettingsViewModel::itemPropertyChanged(const ComponentModel::PropertyChangedSignalArgs& args)
{
    if (args.property() == "state")
    {
        if (m_mediaList.count() == 0)
            return;

        for (int i = 0; i < m_mediaList.count(); ++i)
        {
            if (m_mediaList[i]->state() == MediaItemViewModel::Parsing)
                return;
        }

        retreived();
    }
}


void DownloadSettingsViewModel::browse()
{
    if (m_dialog.isNull())
        return;

    QString selected = outputPath();

    if (m_mediaList.count() == 1 && !m_smartMode)
    {
        // Clip
        QString filters = (detailsIndex() != -1 && m_detailsList[detailsIndex()]->downloadType() == ViewModel::ExtractAudio) ?
                    tr("Audio (*.mp3);;All Files (*.*)") : tr("Video (*.flv *.mp4);;All Files (*.*)");

        if (m_dialog.data()->showSaveDialog(selected, filters, NULL, outputPath(),
                                            tr("Save to"), QFileDialog::DontConfirmOverwrite) == QDialog::Accepted)
        {
            setOutputPath(selected);
        }
    }
    else
    {
        // Playlist
        if (m_dialog.data()->showDirectoryDialog(selected, outputPath(), tr("Save to")) == QDialog::Accepted)
            setOutputPath(selected);
    }
}


void DownloadSettingsViewModel::checkAll()
{
    for (int i = 0; i < m_mediaList.count(); ++i)
        m_mediaList[i]->setChecked(true);
}


void DownloadSettingsViewModel::uncheckAll()
{
    for (int i = 0; i < m_mediaList.count(); ++i)
        m_mediaList[i]->setChecked(false);
}


bool DownloadSettingsViewModel::event(QEvent* e)
{
    switch (e->type())
    {
    case ParseResultEventType:
        {
            ParseResultEvent* pe = (ParseResultEvent*)e;
            parsed(pe->result(), pe->error());
        }
        return true;

    default:
        return false;
    }
}

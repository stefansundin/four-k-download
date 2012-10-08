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


#include "viewmodel/smartmodeviewmodel.h"
#include "viewmodel/smartmodeviewmodel.h"
#include <QSettings>
#include <QDesktopServices>
#include "openmedia/DTMediaDownloader.h"

using namespace ViewModel;
using namespace Mvvm;
using namespace ComponentModel;
using namespace openmedia;


SmartModeViewModel::SmartModeViewModel(const Mvvm::Dialog* dialog, QObject *parent) :
    ComponentModel::NotifyObject(parent),
    m_dialog(dialog),
    m_smartMode(false),
    m_dirName(""),
    m_qualityIndex(-1),
    m_formatIndex(-1),
    m_dirAction(false)
{
    m_dirAction.setEnabled(true);
    m_dirAction.setText(tr("..."));
    m_dirAction.setToolTip(tr("Change output path"));
    QObject::connect(&m_dirAction, SIGNAL(triggered()), this, SLOT(browse()));

    initialize();
}


SmartModeViewModel::~SmartModeViewModel()
{
}


bool SmartModeViewModel::isSmartMode() const
{
    return m_smartMode;
}


void SmartModeViewModel::setSmartMode(bool value)
{
    if (m_smartMode != value)
    {
        m_smartMode = value;
        emitPropertyChanged("smartMode", isSmartMode());
    }
}


QString SmartModeViewModel::dirName() const
{
    return m_dirName;
}


void SmartModeViewModel::setDirName(QString value)
{
    m_dirName = value;
    emitPropertyChanged("dirName", dirName());
}


QStringList SmartModeViewModel::qualityList() const
{
    QStringList result;
    result.append(tr("Original"));
    result.append(tr("1080p"));
    result.append(tr("720p"));
    result.append(tr("480p"));
    result.append(tr("360p"));
    result.append(tr("240p"));
    result.append(tr("QCIF"));
    return result;
}


int SmartModeViewModel::qualityIndex() const
{
    return m_qualityIndex;
}


void SmartModeViewModel::setQualityIndex(int value)
{
    if (value < 0 || value > 6)
        return;

    m_qualityIndex = value;

    emitPropertyChanged("qualityIndex", qualityIndex());
}


bool SmartModeViewModel::qualityEnabled() const
{
    return m_formatIndex != 3;
}


QStringList SmartModeViewModel::formatList() const
{
    QStringList result;
    result.append(tr("FLV"));
    result.append(tr("MP4"));
    result.append(tr("MKV"));
    result.append(tr("MP3"));
    result.append(tr("3GP"));
    return result;
}


int SmartModeViewModel::formatIndex() const
{
    return m_formatIndex;
}


void SmartModeViewModel::setFormatIndex(int value)
{
    if (value < 0 || value > 3)
        return;

    m_formatIndex = value;

    emitPropertyChanged("formatIndex", formatIndex());
    emitPropertyChanged("qualityEnabled", qualityEnabled());
}


QAction* SmartModeViewModel::dirAction()
{
    return &m_dirAction;
}


void SmartModeViewModel::initialize()
{
    QSettings settings;
    downloader::media_quality_type_t quality = (downloader::media_quality_type_t)settings.value("Download/defaultMediaQuality", (int)downloader::mediaQuality480P).toInt();
    downloader::media_content_type_t content = (downloader::media_content_type_t)settings.value("Download/defaultMediaContent", (int)downloader::mediaContentVideoMP4).toInt();
    MediaDownloadType type = (MediaDownloadType)settings.value("Download/defaultDownloadType", (int)DownloadVideo).toInt();

    switch (quality)
    {
    case downloader::mediaQualityOriginal:
        m_qualityIndex = 0;
        break;

    case downloader::mediaQuality1080P:
        m_qualityIndex = 1;
        break;

    case downloader::mediaQuality720P_1:
    case downloader::mediaQuality720P:
        m_qualityIndex = 2;
        break;

    case downloader::mediaQuality480P:
    default:
        m_qualityIndex = 3;
        break;

    case downloader::mediaQuality360P_1:
    case downloader::mediaQuality360P:
        m_qualityIndex = 4;
        break;

    case downloader::mediaQuality240P_1:
    case downloader::mediaQuality240P:
        m_qualityIndex = 5;
        break;
        
    case downloader::mediaQualityQCIF:
        m_qualityIndex = 6;
        break;        
    }


    if (type == ExtractAudio)
    {
        m_formatIndex = 3;
    }
    else
    {
        switch (content)
        {
        case downloader::mediaContentVideoFlv:
            m_formatIndex = 0;
            break;

        case downloader::mediaContentVideoMP4:
        default:
            m_formatIndex = 1;
            break;

        case downloader::mediaContentVideoWebm:
            m_formatIndex = 2;
            break;
            
        case downloader::mediaContentVideo3GP:
            m_formatIndex = 4;
            break;            
        }
    }


    QString path = QDesktopServices::storageLocation(QDesktopServices::MoviesLocation);
    m_dirName = settings.value("Download/outputDirName", path).toString();
    m_smartMode = settings.value("Download/smartMode", false).toBool();
}


void SmartModeViewModel::saveState()
{
    QSettings settings;

    switch (m_qualityIndex)
    {
    case 0:
        settings.setValue("Download/defaultMediaQuality", (int)downloader::mediaQualityOriginal);
        break;

    case 1:
        settings.setValue("Download/defaultMediaQuality", (int)downloader::mediaQuality1080P);
        break;

    case 2:
        settings.setValue("Download/defaultMediaQuality", (int)downloader::mediaQuality720P);
        break;

    case 3:
    default:
        settings.setValue("Download/defaultMediaQuality", (int)downloader::mediaQuality480P);
        break;

    case 4:
        settings.setValue("Download/defaultMediaQuality", (int)downloader::mediaQuality360P);
        break;

    case 5:
        settings.setValue("Download/defaultMediaQuality", (int)downloader::mediaQuality240P);
        break;
        
    case 6:
        settings.setValue("Download/defaultMediaQuality", (int)downloader::mediaQualityQCIF);
        break;        
    }


    switch (m_formatIndex)
    {
    case 0:
        settings.setValue("Download/defaultMediaContent", (int)downloader::mediaContentVideoFlv);
        settings.setValue("Download/defaultDownloadType", (int)DownloadVideo);
        break;

    case 1:
        settings.setValue("Download/defaultMediaContent", (int)downloader::mediaContentVideoMP4);
        settings.setValue("Download/defaultDownloadType", (int)DownloadVideo);
        break;

    case 2:
        settings.setValue("Download/defaultMediaContent", (int)downloader::mediaContentVideoWebm);
        settings.setValue("Download/defaultDownloadType", (int)DownloadVideo);
        break;

    case 3:
        settings.setValue("Download/defaultMediaContent", (int)downloader::mediaContentVideoMP4);
        settings.setValue("Download/defaultDownloadType", (int)ExtractAudio);
        break;
        
    case 4:
        settings.setValue("Download/defaultMediaContent", (int)downloader::mediaContentVideo3GP);
        settings.setValue("Download/defaultDownloadType", (int)DownloadVideo);
        break;
    
    }


    settings.setValue("Download/outputDirName", m_dirName);
    settings.setValue("Download/smartMode", m_smartMode);
}


void SmartModeViewModel::browse()
{
    if (m_dialog.isNull())
        return;

    QString selected = dirName();

    if (m_dialog.data()->showDirectoryDialog(selected, dirName(), tr("Output Path")) == QDialog::Accepted)
        setDirName(selected);
}

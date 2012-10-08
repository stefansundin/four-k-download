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


#ifndef DOWNLOADSETTINGSVIEWMODEL_H
#define DOWNLOADSETTINGSVIEWMODEL_H

#include <QObject>
#include <QList>
#include <QAction>
#include "mvvm/cxx/dialog.h"
#include "componentmodel/notifyobject.h"
#include "viewmodel/mediaitemviewmodel.h"
#include "viewmodel/mediadetailsviewmodel.h"
#include "openmedia/DTMediaDownloader.h"

namespace ViewModel
{

class DownloadSettingsViewModel : public ComponentModel::NotifyObject
{
    Q_OBJECT
    Q_PROPERTY(QList< QSharedPointer<ViewModel::MediaItemViewModel> > mediaList READ mediaList)
    Q_PROPERTY(QList< QSharedPointer<ViewModel::MediaDetailsViewModel> > detailsList READ detailsList)
    Q_PROPERTY(int detailsIndex READ detailsIndex WRITE setDetailsIndex)
    Q_PROPERTY(QStringList subtitlesList READ subtitlesList)
    Q_PROPERTY(int subtitlesIndex READ subtitlesIndex WRITE setSubtitlesIndex)
    Q_PROPERTY(bool subtitlesDownload READ isSubtitlesDownload WRITE setSubtitlesDownload)
    Q_PROPERTY(bool subtitlesEnabled READ isSubtitlesEnabled)
    Q_PROPERTY(QString outputPath READ outputPath WRITE setOutputPath)
    Q_PROPERTY(QAction* outputPathAction READ outputPathAction)
    Q_PROPERTY(QAction* checkAllAction READ checkAllAction)
    Q_PROPERTY(QAction* uncheckAllAction READ uncheckAllAction)
    Q_PROPERTY(State state READ state)
    Q_ENUMS(State)

public:
    enum State { Parsing, Retreiving, Done, Error };

    DownloadSettingsViewModel(const Mvvm::Dialog* dialog, QString url, bool isPlaylist = false, QObject *parent = 0);
    virtual ~DownloadSettingsViewModel();

    QList< QSharedPointer<ViewModel::MediaItemViewModel> > mediaList() const;
    QList< QSharedPointer<ViewModel::MediaDetailsViewModel> > detailsList() const;
    int detailsIndex() const;
    void setDetailsIndex(int value);
    QStringList subtitlesList() const;
    int subtitlesIndex() const;
    void setSubtitlesIndex(int value);
    bool isSubtitlesDownload() const;
    void setSubtitlesDownload(bool value);
    bool isSubtitlesEnabled() const;
    QString outputPath() const;
    void setOutputPath(QString value);
    QAction* outputPathAction();
    QAction* checkAllAction();
    QAction* uncheckAllAction();
    State state() const;

    MediaDownloadList downloadList() const;
    void saveSubtitles() const;

    Q_INVOKABLE bool canClose();

protected:
    void initialize();
    void deinitialize();
    void saveDefaults();
    void restoreDefaults();

    virtual bool event(QEvent* e);

protected slots:
    void parsed(openmedia::downloader::url_parser_result_ptr result, openmedia::downloader::url_parser::ErrorCode error);
    void retreived();
    void itemPropertyChanged(const ComponentModel::PropertyChangedSignalArgs& args);
    void browse();
    void checkAll();
    void uncheckAll();

private:
    QWeakPointer<const Mvvm::Dialog> m_dialog;
    openmedia::downloader::url_parser_result_ptr m_sdkResult;
    QList< QSharedPointer<ViewModel::MediaItemViewModel> > m_mediaList;
    QList< QSharedPointer<ViewModel::MediaDetailsViewModel> > m_detailsList;
    int m_detailsIndex;
    QStringList m_subtitlesList;
    int m_subtitlesIndex;
    bool m_subtitlesDownload;
    bool m_smartMode;
    QString m_outputDirName;
    QString m_outputFileName;
    QAction m_outputPathAction;
    QAction m_checkAllAction;
    QAction m_uncheckAllAction;
    State m_state;
};

} // ViewModel

Q_DECLARE_METATYPE(ViewModel::DownloadSettingsViewModel::State)

#endif // DOWNLOADSETTINGSVIEWMODEL_H

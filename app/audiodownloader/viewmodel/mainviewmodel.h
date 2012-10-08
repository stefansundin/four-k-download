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


#ifndef MAINVIEWMODEL_H
#define MAINVIEWMODEL_H

#include <QObject>
#include <QAction>
#include "viewmodel/downloadlistviewmodel.h"
#include "mvvm/cxx/dialog.h"
#include "componentmodel/notifyobject.h"
#include "componentmodel/notifyselection.h"
#include "componentmodel/propertybinding.h"
#include "componentmodel/objectlessor.h"
#include "multimedia/audioplayer.h"
#include "openmedia/DTMediaDownloader.h"

namespace ViewModel
{

class MainViewModel : public ComponentModel::NotifyObject
{
    Q_OBJECT
    Q_PROPERTY(QAction* pasteAction READ pasteAction)
    Q_PROPERTY(QAction* browseAction READ browseAction)
    Q_PROPERTY(QAction* informationAction READ informationAction)
    Q_PROPERTY(QAction* removeAction READ removeAction)
    Q_PROPERTY(QAction* clearAction READ clearAction)
    Q_PROPERTY(QAction* pauseAllAction READ pauseAllAction)
    Q_PROPERTY(QAction* resumeAllAction READ resumeAllAction)
    Q_PROPERTY(QAction* facebookAction READ facebookAction)

public:
    MainViewModel(const Mvvm::Dialog* dialog, QObject* parent = 0);
    virtual ~MainViewModel();

    DownloadListViewModel* list();
    ComponentModel::NotifySelection* selection();
    QAction* pasteAction();
    QAction* browseAction();
    QAction* informationAction();
    QAction* removeAction();
    QAction* clearAction();
    QAction* pauseAllAction();
    QAction* resumeAllAction();
    QAction* facebookAction();

    Q_INVOKABLE bool canClose();

public slots:
    void showMessage(QString message) const;
    void showUpdate(QString message, bool& needUpdate) const;

signals:
    void downloadCompleted();

protected:
    void save();
    void restore();

    void updateActions(int index);

protected slots:
    void paste();
    void browse();
    void information();
    void removeItem();
    void clearItems();
    void pauseItems();
    void resumeItems();
    void openFacebook();

    void onListChanged(const ComponentModel::ListChangedSignalArgs& args);
    void onSelectionChanged(const ComponentModel::SelectionChangedSignalArgs& args);
    void onItemPropertyChanged(const ComponentModel::PropertyChangedSignalArgs& args);
    void onItemParsed(openmedia::downloader::url_parser_result_ptr result);
    void onItemDownloadCompleted();
    void onItemPlayerCompleted();

    void updatePasteAction();

private:
    QWeakPointer<const Mvvm::Dialog> m_dialog;
    QScopedPointer<Multimedia::AudioPlayer> m_player;
    QSharedPointer<ComponentModel::ObjectLessor> m_playerLessor;
    DownloadListViewModel m_list;
    ComponentModel::NotifySelection m_selection;
    QAction m_pasteAction;
    QAction m_browseAction;
    QAction m_informationAction;
    QAction m_removeAction;
    QAction m_clearAction;
    QAction m_pauseAllAction;
    QAction m_resumeAllAction;
    QAction m_facebookAction;
};

} // ViewModel

#endif // MAINVIEWMODEL_H

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
#include <QDeclarativeListProperty>
#include "viewmodel/mediaitemviewmodel.h"
#include "componentmodel/notifylistmodel.h"
#include "componentmodel/imageprovider.h"
#include "componentmodel/objectlessor.h"
#include "multimedia/audioplayer.h"


namespace ViewModel
{

class MainViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QDeclarativeImageProvider* thumbnails READ thumbnails)
    Q_PROPERTY(QObject* mediaListModel READ mediaListModel NOTIFY fakeSignal)
    Q_PROPERTY(int mediaIndex READ mediaIndex WRITE setMediaIndex NOTIFY mediaIndexChanged)
    Q_PROPERTY(QString outputPath READ outputPath WRITE setOutputPath NOTIFY outputPathChanged)
    Q_PROPERTY(QAction* deleteItemAction READ deleteItemAction NOTIFY fakeSignal)
    Q_PROPERTY(QAction* clearItemsAction READ clearItemsAction NOTIFY fakeSignal)
    Q_PROPERTY(QAction* pauseItemsAction READ pauseItemsAction NOTIFY fakeSignal)
    Q_PROPERTY(QAction* resumeItemsAction READ resumeItemsAction NOTIFY fakeSignal)

public:
    explicit MainViewModel(QObject *parent = 0);
    ~MainViewModel();

    QDeclarativeImageProvider* thumbnails();
    QDeclarativeImageProvider* banners();
    QObject* mediaListModel();
    int mediaIndex() const;
    void setMediaIndex(int value);
    QString outputPath() const;
    void setOutputPath(QString value);
    QAction* deleteItemAction();
    QAction* clearItemsAction();
    QAction* pauseItemsAction();
    QAction* resumeItemsAction();

    Q_INVOKABLE void addItem(QString path);
    Q_INVOKABLE void showHelp() const;
    Q_INVOKABLE void showFacebook() const;
    Q_INVOKABLE bool isIdle();

protected:
    void save();
    void restore();

public slots:
    void deleteItem();
    void clearItems();
    void pauseItems();
    void resumeItems();
    void receiveMessage(QString message);
    void receiveUpdate(QString message, bool& needUpdate);
    void alert() const;

signals:
    void mediaIndexChanged(int value);
    void outputPathChanged(QString value);
    void fakeSignal(); // Disable qml warnings
    void messageReceived(QString message);
    void updateReceived(QString message, QObject* retVal);

protected slots:
    void onMediaListChanged(const ComponentModel::ListChangedSignalArgs& args);
    void onMediaItemStateChanged(MediaItemViewModel::State state);
    void onMediaItemConversionCompleted();
    void onMediaItemPlayerCompleted();

private:
    QSharedPointer<ComponentModel::ImageProvider> m_thumbnails;
    QSharedPointer<ComponentModel::ImageProvider> m_banners;
    QScopedPointer<Multimedia::AudioPlayer> m_player;
    QSharedPointer<ComponentModel::ObjectLessor> m_playerLessor;
    QScopedPointer<ComponentModel::NotifyList> m_mediaList;
    QScopedPointer<ComponentModel::NotifyListModel> m_mediaListModel;
    int m_mediaIndex;
    QAction m_deleteItemAction;
    QAction m_clearItemsAction;
    QAction m_pauseItemsAction;
    QAction m_resumeItemsAction;
};

} // ViewModel

#endif // MAINVIEWMODEL_H

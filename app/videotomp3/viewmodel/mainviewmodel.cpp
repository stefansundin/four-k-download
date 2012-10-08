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


#include "viewmodel/mainviewmodel.h"
#include "componentmodel/filesystem.h"

using namespace ViewModel;
using namespace ComponentModel;
using namespace Multimedia;


Q_DECLARE_METATYPE(QDeclarativeImageProvider*)


MainViewModel::MainViewModel(QObject *parent) :
    QObject(parent),
    m_thumbnails(new ImageProvider()),
    m_banners(new ImageProvider()),
    m_player(new AudioPlayer()),
    m_playerLessor(new ObjectLessor(m_player.data())),
    m_mediaList(new NotifyList()),
    m_mediaListModel(new NotifyListModel(m_mediaList.data())),
    m_mediaIndex(-1),
    m_deleteItemAction(this),
    m_clearItemsAction(this),
    m_pauseItemsAction(this),
    m_resumeItemsAction(this)
{
    m_deleteItemAction.setEnabled(false);
    QObject::connect(&m_deleteItemAction, SIGNAL(triggered()), this, SLOT(deleteItem()));

    m_clearItemsAction.setEnabled(false);
    QObject::connect(&m_clearItemsAction, SIGNAL(triggered()), this, SLOT(clearItems()));

    m_pauseItemsAction.setEnabled(false);
    QObject::connect(&m_pauseItemsAction, SIGNAL(triggered()), this, SLOT(pauseItems()));

    m_resumeItemsAction.setEnabled(false);
    QObject::connect(&m_resumeItemsAction, SIGNAL(triggered()), this, SLOT(resumeItems()));

    QObject::connect(m_mediaList.data(), SIGNAL(listChanged(ComponentModel::ListChangedSignalArgs)),
                     this, SLOT(onMediaListChanged(ComponentModel::ListChangedSignalArgs)));

    restore();
}


MainViewModel::~MainViewModel()
{
    save();
}


void MainViewModel::save()
{
    QSettings settings;
    QString filename = settings.value("Convert/convertedItems").toString();

    try
    {        
        if (filename.isEmpty())
        {
            QDir directory(QDesktopServices::storageLocation(QDesktopServices::DataLocation));
            QDir().mkpath(directory.absolutePath());
            filename = FileSystem::generateFileName(".xml");
            filename = directory.filePath(filename);
        }

        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly))
            return;

        QDomDocument doc;
        QDomElement root = doc.createElement("Items");
        doc.appendChild(root);

        for (int i = 0; i < m_mediaList->count(); ++i)
        {
            QSharedPointer<MediaItemViewModel> item = qobject_cast<MediaItemViewModel*>(m_mediaList->at(i));

            if (item->state() == MediaItemViewModel::DoneState || 
                item->state() == MediaItemViewModel::PlayerPlayState || 
                item->state() == MediaItemViewModel::PlayerPauseState)
            {
                QDomElement element;
                if (item->serialize(doc, element))
                    root.appendChild(element);
            }
        }

        QTextStream out(&file);
        doc.save(out, 4);

        settings.setValue("Convert/convertedItems", filename);
    }
    catch(...)
    {
    }
}


void MainViewModel::restore()
{
    QSettings settings;
    QString filename = settings.value("Convert/convertedItems").toString();
\
    try
    {
        if (filename.isEmpty())
            return;

        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly))
            return;

        QDomDocument doc;
        if (!doc.setContent(&file))
            return;

        QDomElement root = doc.documentElement();

        for (QDomElement e = root.lastChildElement(); !e.isNull(); e = e.previousSiblingElement())
        {
            QSharedPointer<MediaItemViewModel> item(new MediaItemViewModel(m_thumbnails, m_playerLessor));

            if (!item->deserialize(doc, e))
                continue;

            m_mediaList.data()->insert(0, item);
        }
    }
    catch(...)
    {
    }
}


QDeclarativeImageProvider* MainViewModel::thumbnails()
{
    return m_thumbnails.data();
}


QDeclarativeImageProvider* MainViewModel::banners()
{
    return m_banners.data();
}


QObject* MainViewModel::mediaListModel()
{
    return m_mediaListModel.data();
}


int MainViewModel::mediaIndex() const
{
    return m_mediaIndex;
}


void MainViewModel::setMediaIndex(int value)
{
    if (m_mediaIndex != value)
    {
        m_mediaIndex = value;
        emit mediaIndexChanged(value);

        m_deleteItemAction.setEnabled(m_mediaIndex >= 0);
    }
}


QString MainViewModel::outputPath() const
{
    QSettings settings;
    QString path = QDesktopServices::storageLocation(QDesktopServices::MusicLocation);
    return settings.value("Download/outputPath", path).toString();
}


void MainViewModel::setOutputPath(QString value)
{
    QSettings settings;
    settings.setValue("Download/outputPath", value);

    emit outputPathChanged(value);
}


QAction* MainViewModel::deleteItemAction()
{
    return &m_deleteItemAction;
}


QAction* MainViewModel::clearItemsAction()
{
    return &m_clearItemsAction;
}


QAction* MainViewModel::pauseItemsAction()
{
    return &m_pauseItemsAction;
}


QAction* MainViewModel::resumeItemsAction()
{
    return &m_resumeItemsAction;
}


void MainViewModel::addItem(QString path)
{
    MediaItemViewModel* item = new MediaItemViewModel(path, outputPath(), m_thumbnails, m_playerLessor);
    QObject::connect(item, SIGNAL(stateChanged(MediaItemViewModel::State)),
                     this, SLOT(onMediaItemStateChanged(MediaItemViewModel::State)));
    QObject::connect(item, SIGNAL(playerCompleted()),
                     this, SLOT(onMediaItemPlayerCompleted()));
    QObject::connect(item, SIGNAL(conversionCompleted()),
                     this, SLOT(onMediaItemConversionCompleted()));

    m_mediaList.data()->insert(0, QSharedPointer<MediaItemViewModel>(item));
}


void MainViewModel::deleteItem()
{
    if (m_mediaIndex == -1)
        return;

    m_mediaList.data()->remove(m_mediaIndex);
}


void MainViewModel::clearItems()
{
    m_mediaList.data()->clear();
}


void MainViewModel::pauseItems()
{
    for (int i = 0; i < m_mediaList.data()->count(); ++i)
        qobject_cast<MediaItemViewModel*>(m_mediaList->at(i))->pause();
}


void MainViewModel::resumeItems()
{
    for (int i = 0; i < m_mediaList.data()->count(); ++i)
        qobject_cast<MediaItemViewModel*>(m_mediaList->at(i))->resume();
}


void MainViewModel::showHelp() const
{
    QSettings settings;
    QString web = settings.value("webHelp").toString();

    QDesktopServices::openUrl(QUrl(web));
}


void MainViewModel::showFacebook() const
{
    QSettings settings;
    QString url = settings.value("webFacebook").toString();

    QDesktopServices::openUrl(QUrl(url));
}


bool MainViewModel::isIdle()
{
    for (int i = 0; i < m_mediaList->count(); ++i)
    {
        switch (qobject_cast<MediaItemViewModel*>(m_mediaList->at(i))->state())
        {
        case MediaItemViewModel::OpenState:
        case MediaItemViewModel::ConvertState:
        case MediaItemViewModel::PauseState:
            return false;

        default:
            break;
        }
    }

    return true;
}


void MainViewModel::receiveMessage(QString message)
{
    emit messageReceived(message);
}


void MainViewModel::receiveUpdate(QString message, bool& needUpdate)
{
    QScopedPointer<QObject> retVal(new QObject);
    retVal->setProperty("needUpdate", needUpdate);

    emit updateReceived(message, retVal.data());

    needUpdate = retVal->property("needUpdate").toBool();
}


void MainViewModel::alert() const
{
    QApplication::alert(QApplication::activeWindow());
}


void MainViewModel::onMediaListChanged(const ComponentModel::ListChangedSignalArgs& args)
{
    Q_UNUSED(args);

    m_clearItemsAction.setEnabled(m_mediaList->count() > 0);
    m_pauseItemsAction.setEnabled(m_mediaList->count() > 0);
    m_resumeItemsAction.setEnabled(m_mediaList->count() > 0);
}


void MainViewModel::onMediaItemStateChanged(MediaItemViewModel::State state)
{
}


void MainViewModel::onMediaItemConversionCompleted()
{
    save();

    if (isIdle())
        alert();
}


void MainViewModel::onMediaItemPlayerCompleted()
{
    if (MediaItemViewModel* item = qobject_cast<MediaItemViewModel*>(sender()))
    {
        int index = m_mediaList->indexOf(item);
        if (index != -1)
        {
            for (int i = index + 1; i < m_mediaList->count(); ++i)
            {
                MediaItemViewModel* item = qobject_cast<MediaItemViewModel*>(m_mediaList->at(i).data());
                if (item->togglePlayAction()->isEnabled())
                {
                    item->togglePlay();
                    break;
                }
            }
        }
    }
}

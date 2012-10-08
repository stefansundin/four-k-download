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
#include "viewmodel/downloaditemviewmodel.h"
#include "openmedia/DTMediaSite.h"
#include <QDesktopServices>
#include <QUrl>
#include <QClipboard>
#include <QApplication>
#include <QSettings>
#include "boost/filesystem.hpp"

using namespace ViewModel;
using namespace Mvvm;
using namespace ComponentModel;
using namespace Multimedia;
using namespace openmedia;


MainViewModel::MainViewModel(const Mvvm::Dialog* dialog, QObject* parent) :
    NotifyObject(parent),
    m_dialog(dialog),
    m_player(new AudioPlayer()),
    m_playerLessor(new ObjectLessor(m_player.data())),
    m_list(this),
    m_selection(&m_list, this),
    m_pasteAction(this),
    m_browseAction(this),
    m_informationAction(this),
    m_removeAction(this),
    m_clearAction(this),
    m_pauseAllAction(this),
    m_resumeAllAction(this),
    m_facebookAction(this)
{
    m_pasteAction.setEnabled(true);
    m_pasteAction.setText(tr("Paste url"));
    m_pasteAction.setToolTip(tr("Paste video link from clipboard"));
    m_pasteAction.setShortcut(QKeySequence::Paste);
    m_pasteAction.setIcon(QIcon(":/image/paste-url"));
    QObject::connect(&m_pasteAction, SIGNAL(triggered()), this, SLOT(paste()));

    m_browseAction.setEnabled(true);
    m_browseAction.setText(tr("Change folder"));
    m_browseAction.setToolTip(tr("Change folder for output audio files"));
    m_browseAction.setIcon(QIcon(":/image/change-folder"));
    QObject::connect(&m_browseAction, SIGNAL(triggered()), this, SLOT(browse()));

    m_informationAction.setEnabled(true);
    m_informationAction.setText(tr("Help"));
    m_informationAction.setToolTip(tr("Show online help"));
    m_informationAction.setIcon(QIcon(":/image/help"));
    QObject::connect(&m_informationAction, SIGNAL(triggered()), this, SLOT(information()));

    m_removeAction.setEnabled(false);
    m_removeAction.setText(tr("Remove"));
    m_removeAction.setToolTip("");
    m_removeAction.setIcon(QIcon(":/image/remove"));
    m_removeAction.setShortcut(QKeySequence::Delete);
    QObject::connect(&m_removeAction, SIGNAL(triggered()), this, SLOT(removeItem()));

    m_clearAction.setEnabled(false);
    m_clearAction.setText(tr("Remove all"));
    m_clearAction.setToolTip(tr("Remove all downloads from list"));
    QObject::connect(&m_clearAction, SIGNAL(triggered()), this, SLOT(clearItems()));

    m_pauseAllAction.setEnabled(false);
    m_pauseAllAction.setText(tr("Pause all"));
    m_pauseAllAction.setToolTip(tr(""));
    QObject::connect(&m_pauseAllAction, SIGNAL(triggered()), this, SLOT(pauseItems()));

    m_resumeAllAction.setEnabled(false);
    m_resumeAllAction.setText(tr("Resume all"));
    m_resumeAllAction.setToolTip(tr(""));
    QObject::connect(&m_resumeAllAction, SIGNAL(triggered()), this, SLOT(resumeItems()));

    m_facebookAction.setEnabled(true);
    m_facebookAction.setText(tr("Like"));
    m_facebookAction.setToolTip(tr("Show 4K Download Facebook page"));
    m_facebookAction.setIcon(QIcon(":/image/facebook"));
    QObject::connect(&m_facebookAction, SIGNAL(triggered()), this, SLOT(openFacebook()));

    QObject::connect(&m_list, SIGNAL(listChanged(ComponentModel::ListChangedSignalArgs)),
                     this, SLOT(onListChanged(ComponentModel::ListChangedSignalArgs)));
    QObject::connect(&m_selection, SIGNAL(selectionChanged(ComponentModel::SelectionChangedSignalArgs)),
                     this, SLOT(onSelectionChanged(ComponentModel::SelectionChangedSignalArgs)));

    restore();

    QTimer *timer = new QTimer(this);
    connect (timer, SIGNAL (timeout()), this, SLOT (updatePasteAction()));
    timer->start(1000);
}


MainViewModel::~MainViewModel()
{
    openmedia::downloader::media_downloader::shutdown();
    save();
}


void MainViewModel::save()
{
    QSettings settings;
    QString filename = settings.value("Download/downloadedItems").toString();

    try
    {
        if (filename.isEmpty())
        {
            QDir directory(QDesktopServices::storageLocation(QDesktopServices::DataLocation));
            QDir().mkpath(directory.absolutePath());
            filename = QUuid::createUuid().toString();
            filename = filename.remove("{", Qt::CaseInsensitive);
            filename = filename.remove("}", Qt::CaseInsensitive);
            filename = directory.filePath(filename + ".xml");
        }

        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly))
            return;

        QDomDocument doc;
        QDomElement root = doc.createElement("Items");
        doc.appendChild(root);

        for (int i = 0; i < m_list.count(); ++i)
        {
            QSharedPointer<DownloadItemViewModel> item = m_list.at(i).staticCast<DownloadItemViewModel>();

            if (item->state() == DownloadItemViewModel::DoneState || 
                item->state() == DownloadItemViewModel::PlayerPlayState || 
                item->state() == DownloadItemViewModel::PlayerPauseState)
            {
                QDomElement element;
                if (item->serialize(doc, element))
                    root.appendChild(element);
            }
        }


        QTextStream out(&file);
        doc.save(out, 4);

        settings.setValue("Download/downloadedItems", filename);
    }
    catch(...)
    {
    }
}


void MainViewModel::restore()
{
    QSettings settings;
    QString filename = settings.value("Download/downloadedItems").toString();

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
            QSharedPointer<DownloadItemViewModel> item(new DownloadItemViewModel(m_dialog.data(), m_playerLessor));

            if (!item->deserialize(doc, e))
                continue;

            QObject::connect(item.data(), SIGNAL(propertyChanged(ComponentModel::PropertyChangedSignalArgs)),
                             this, SLOT(onItemPropertyChanged(ComponentModel::PropertyChangedSignalArgs)));
            m_list.insert(0, item);
        }
    }
    catch(...)
    {
    }
}


DownloadListViewModel* MainViewModel::list()
{
    return &m_list;
}


NotifySelection* MainViewModel::selection()
{
    return &m_selection;
}


QAction* MainViewModel::pasteAction()
{
    return &m_pasteAction;
}


QAction* MainViewModel::browseAction()
{
    return &m_browseAction;
}


QAction* MainViewModel::informationAction()
{
    return &m_informationAction;
}


QAction* MainViewModel::removeAction()
{
    return &m_removeAction;
}


QAction* MainViewModel::clearAction()
{
    return &m_clearAction;
}


QAction* MainViewModel::pauseAllAction()
{
    return &m_pauseAllAction;
}


QAction* MainViewModel::resumeAllAction()
{
    return &m_resumeAllAction;
}


QAction* MainViewModel::facebookAction()
{
    return &m_facebookAction;
}


bool MainViewModel::canClose()
{
    bool allDone = true;

    for (int i = 0; i < m_list.count(); ++i)
    {
        QSharedPointer<DownloadItemViewModel> item = m_list.at(i).staticCast<DownloadItemViewModel>();
        DownloadItemViewModel::State state = item->state();
        if (state == DownloadItemViewModel::DownloadState || state == DownloadItemViewModel::ConvertState || state == DownloadItemViewModel::PauseState)
        {
            allDone = false;
            break;
        }
    }

    if (!allDone)
    {
        QList<Mvvm::Dialog::MessageButton> buttons;
        buttons << Mvvm::Dialog::MessageButton(tr("Quit"), QMessageBox::AcceptRole);
        buttons << Mvvm::Dialog::MessageButton(tr("Cancel"), QMessageBox::RejectRole);

        int result = m_dialog.data()->showMessageDialog(tr("Some downloads are not finished yet. Are you sure want to quit?"),
                                                        QMessageBox::Question, QCoreApplication::applicationName(), buttons, 1);
        allDone = (result == 0);
    }

    return allDone;
}


void MainViewModel::showMessage(QString message) const
{
    QList<Mvvm::Dialog::MessageButton> buttons;
    buttons << Mvvm::Dialog::MessageButton(tr("Close"), QMessageBox::RejectRole);

    m_dialog.data()->showMessageDialog(message, QMessageBox::Information, QCoreApplication::applicationName(), buttons, 0);
}


void MainViewModel::showUpdate(QString message, bool& needUpdate) const
{
    QList<Mvvm::Dialog::MessageButton> buttons;
    buttons << Mvvm::Dialog::MessageButton(tr("Open Site"), QMessageBox::AcceptRole);
    buttons << Mvvm::Dialog::MessageButton(tr("Close"), QMessageBox::RejectRole);

    int result = m_dialog.data()->showMessageDialog(message, QMessageBox::Information, QCoreApplication::applicationName(), buttons, 0);

    needUpdate = (result == 0);
}


void MainViewModel::paste()
{
    QClipboard* clipboard = QApplication::clipboard();
    QString url = clipboard->text();

    try
    {
        // Validate url

        downloader::media_site_type_t type = downloader::media_site_utils::validate_url(url.toStdString());

        switch (type)
        {
        case downloader::mediaSiteNull:
            {
                QList<Mvvm::Dialog::MessageButton> buttons;
                buttons << Mvvm::Dialog::MessageButton(tr("Close"), QMessageBox::RejectRole);

                m_dialog.data()->showMessageDialog(tr("Copy video link from your browser address bar and then click \"Paste URL\" here."),
                                               QMessageBox::Information, QCoreApplication::applicationName(), buttons, 0);
            }
            return;

        case downloader::mediaSiteUnknown:
            {
                QList<Mvvm::Dialog::MessageButton> buttons;
                buttons << Mvvm::Dialog::MessageButton(tr("Post"), QMessageBox::AcceptRole);
                buttons << Mvvm::Dialog::MessageButton(tr("Close"), QMessageBox::RejectRole);

                int result = m_dialog.data()->showMessageDialog(tr("Sorry this site is unsupported. Please post a request to add this site in supported list."),
                                                   QMessageBox::Information, QCoreApplication::applicationName(), buttons, 0);
            }
            return;

        default:
            break;
        }

        // Check playlist

        bool isPlaylist = downloader::media_site_utils::is_contain_playlist(url.toStdString());

        if (isPlaylist)
        {
            QList<Mvvm::Dialog::MessageButton> buttons;
            buttons << Mvvm::Dialog::MessageButton(tr("Download Playlist"), QMessageBox::AcceptRole);
            buttons << Mvvm::Dialog::MessageButton(tr("Download Clip"), QMessageBox::AcceptRole);
            buttons << Mvvm::Dialog::MessageButton(tr("Cancel"), QMessageBox::RejectRole);

            int result = m_dialog.data()->showMessageDialog(tr("This video is part of the playlist. Do you want do download all playlist or clip only?"),
                                               QMessageBox::Information, QCoreApplication::applicationName(), buttons, 0);

            if (result == 1)
                isPlaylist = false;
            else if (result == 2)
                return;
        }

        // Start download

        QSharedPointer<DownloadItemViewModel> item(new DownloadItemViewModel(m_dialog.data(), url, isPlaylist, m_playerLessor));
        QObject::connect(item.data(), SIGNAL(propertyChanged(ComponentModel::PropertyChangedSignalArgs)),
                         this, SLOT(onItemPropertyChanged(ComponentModel::PropertyChangedSignalArgs)));
        QObject::connect(item.data(), SIGNAL(parsed(openmedia::downloader::url_parser_result_ptr)),
                         this, SLOT(onItemParsed(openmedia::downloader::url_parser_result_ptr)));
        QObject::connect(item.data(), SIGNAL(downloadCompleted()),
                         this, SLOT(onItemDownloadCompleted()));
        QObject::connect(item.data(), SIGNAL(playerCompleted()),
                         this, SLOT(onItemPlayerCompleted()));
        m_list.insert(0, item);
    }
    catch (...)
    {
        QList<Mvvm::Dialog::MessageButton> buttons;
        buttons << Mvvm::Dialog::MessageButton(tr("Close"), QMessageBox::RejectRole);

        m_dialog.data()->showMessageDialog(tr("No valid url found in the clipboard. Please copy video link from the browser address bar and press \"Paste URL\" again."),
                                           QMessageBox::Information, QCoreApplication::applicationName(), buttons, 0);
    }
}


void MainViewModel::browse()
{
    QSettings settings;
    QString path = QDesktopServices::storageLocation(QDesktopServices::MusicLocation);
    path = settings.value("Download/outputDirName", path).toString();

    if (m_dialog.data()->showDirectoryDialog(path, path, tr("Choose output directory")) == QDialog::Accepted)
    {
        settings.setValue("Download/outputDirName", path);
    }
}


void MainViewModel::information()
{
    QSettings settings;
    QString web = settings.value("webHelp").toString();

    QDesktopServices::openUrl(QUrl(web));
}


void MainViewModel::removeItem()
{
    if (m_selection.isValid())
    {
        QSettings settings;

        bool confirm = settings.value("Download/deleteConfirmation", true).toBool();

        if (confirm)
        {
            QList<Mvvm::Dialog::MessageButton> buttons;
            buttons << Mvvm::Dialog::MessageButton(tr("Remove"), QMessageBox::AcceptRole);
            buttons << Mvvm::Dialog::MessageButton(tr("Remove and don't ask again"), QMessageBox::AcceptRole);
            buttons << Mvvm::Dialog::MessageButton(tr("Cancel"), QMessageBox::RejectRole);

            QSharedPointer<DownloadItemViewModel> item = m_list.at(m_selection.currentIndex()).staticCast<DownloadItemViewModel>();
            QString text = tr("Are you sure want to remove \"%1\"?").arg(item->title());

            int result = m_dialog.data()->showMessageDialog(text, QMessageBox::Information, QCoreApplication::applicationName(), buttons, 0);

            if (result == 2)
                return;

            if (result == 1)
                settings.setValue("Download/deleteConfirmation", false);
        }


        m_list.remove(m_selection.currentIndex());
    }
}


void MainViewModel::clearItems()
{
    QSettings settings;

    bool confirm = settings.value("Download/deleteConfirmation", true).toBool();

    if (confirm)
    {
        QList<Mvvm::Dialog::MessageButton> buttons;
        buttons << Mvvm::Dialog::MessageButton(tr("Remove"), QMessageBox::AcceptRole);
        buttons << Mvvm::Dialog::MessageButton(tr("Remove and don't ask again"), QMessageBox::AcceptRole);
        buttons << Mvvm::Dialog::MessageButton(tr("Cancel"), QMessageBox::RejectRole);

        QSharedPointer<DownloadItemViewModel> item = m_list.at(m_selection.currentIndex()).staticCast<DownloadItemViewModel>();
        QString text = tr("Are you sure want to remove all downloads from list?");

        int result = m_dialog.data()->showMessageDialog(text, QMessageBox::Information, QCoreApplication::applicationName(), buttons, 0);

        if (result == 2)
            return;

        if (result == 1)
            settings.setValue("Download/deleteConfirmation", false);
    }

    m_list.clear();
}


void MainViewModel::pauseItems()
{
    for (int i = 0; i < m_list.count(); ++i)
    {
        QSharedPointer<DownloadItemViewModel> item = m_list.at(i).staticCast<DownloadItemViewModel>();
        item->pause();
    }
}


void MainViewModel::resumeItems()
{
    for (int i = 0; i < m_list.count(); ++i)
    {
        QSharedPointer<DownloadItemViewModel> item = m_list.at(i).staticCast<DownloadItemViewModel>();
        item->resume();
    }
}


void MainViewModel::openFacebook()
{
    QSettings settings;
    QString url = settings.value("webFacebook").toString();

    QDesktopServices::openUrl(QUrl(url));
}


void MainViewModel::updateActions(int index)
{
    m_removeAction.setEnabled(index >= 0);
    m_clearAction.setEnabled(m_list.count() > 0);
    m_pauseAllAction.setEnabled(m_list.count() > 0);
    m_resumeAllAction.setEnabled(m_list.count() > 0);
}


void MainViewModel::onItemPropertyChanged(const ComponentModel::PropertyChangedSignalArgs& args)
{
}


void MainViewModel::onListChanged(const ListChangedSignalArgs& args)
{
    Q_UNUSED(args)
    updateActions(m_selection.currentIndex());
}


void MainViewModel::onSelectionChanged(const SelectionChangedSignalArgs& args)
{
    updateActions(args.currentIndex());
}


void MainViewModel::onItemParsed(openmedia::downloader::url_parser_result_ptr result)
{
    for (unsigned int i = 1; i < result->size(); ++i)
    {
        QSharedPointer<DownloadItemViewModel> item(new DownloadItemViewModel(m_dialog.data(), result->at(i), m_playerLessor));
        QObject::connect(item.data(), SIGNAL(propertyChanged(ComponentModel::PropertyChangedSignalArgs)),
                         this, SLOT(onItemPropertyChanged(ComponentModel::PropertyChangedSignalArgs)));
        m_list.insert(i, item);
    }
}


void MainViewModel::onItemDownloadCompleted()
{
    save();

    for (int i = 0; i < m_list.count(); ++i)
    {
        switch (m_list.at(i).staticCast<DownloadItemViewModel>()->state())
        {
        case DownloadItemViewModel::DownloadState:
        case DownloadItemViewModel::ConvertState:
            return;

        default:
            break;
        }
    }

    emit downloadCompleted();
}


void MainViewModel::onItemPlayerCompleted()
{
    if (DownloadItemViewModel* item = qobject_cast<DownloadItemViewModel*>(sender()))
    {
        int index = m_list.indexOf(item);
        if (index != -1)
        {
            for (int i = index + 1; i < m_list.count(); ++i)
            {
                DownloadItemViewModel* item = qobject_cast<DownloadItemViewModel*>(m_list.at(i).data());
                if (item->togglePlayAction()->isEnabled())
                {
                    item->togglePlay();
                    break;
                }
            }
        }
    }
}


void MainViewModel::updatePasteAction()
{
    QString image = ":/image/paste-url";

    try
    {
        QClipboard* clipboard = QApplication::clipboard();
        QString url = clipboard->text();

        downloader::media_site_type_t type = downloader::media_site_utils::validate_url(url.toStdString());

        switch (type)
        {
        case downloader::mediaSiteYoutube:
            image = ":/image/paste-youtube";
            break;

        case downloader::mediaSiteDailymotion:
            image = ":/image/paste-dailymotion";
            break;

        case downloader::mediaSiteFacebook:
            image = ":/image/paste-facebook";
            break;

        case downloader::mediaSiteVimeo:
            image = ":/image/paste-vimeo";
            break;

        case downloader::mediaSiteMetacafe:
            image = ":/image/paste-metacafe";
            break;


        default:
            break;
        }

    }
    catch (...)
    {
    }

    m_pasteAction.setIcon(QIcon(image));
}

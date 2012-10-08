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
#include "viewmodel/downloadsettingsviewmodel.h"
#include "viewmodel/smartmodeviewmodel.h"
#include "openmedia/DTMediaSite.h"
#include "componentmodel/filesystem.h"
#include <QDesktopServices>
#include <QUrl>
#include <QClipboard>
#include <QApplication>
#include <QSettings>
#include "boost/filesystem.hpp"

using namespace ViewModel;
using namespace Mvvm;
using namespace ComponentModel;
using namespace openmedia;


MainViewModel::MainViewModel(const Mvvm::Dialog* dialog, QObject* parent) :
    NotifyObject(parent),
    m_dialog(dialog),
    m_list(this),
    m_selection(&m_list, this),
    m_pasteAction(this),
    m_smartModeAction(this),
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

    m_smartModeAction.setEnabled(true);
    m_smartModeAction.setText(tr("Smart Mode"));
    m_smartModeAction.setToolTip(tr("Show smart mode settings"));
    m_smartModeAction.setIcon(QIcon(":/image/smart-mode-off"));
    QObject::connect(&m_smartModeAction, SIGNAL(triggered()), this, SLOT(showSmartMode()));

    m_informationAction.setEnabled(true);
    m_informationAction.setText(tr("Help"));
    m_informationAction.setToolTip(tr("Show online help"));
    m_informationAction.setIcon(QIcon(":/image/help"));
    QObject::connect(&m_informationAction, SIGNAL(triggered()), this, SLOT(showInformation()));

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
                     this, SLOT(listChanged(ComponentModel::ListChangedSignalArgs)));
    QObject::connect(&m_selection, SIGNAL(selectionChanged(ComponentModel::SelectionChangedSignalArgs)),
                     this, SLOT(selectionChanged(ComponentModel::SelectionChangedSignalArgs)));

    restore();
    updateSmartMode();

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
            filename = FileSystem::generateFileName(".xml");
            filename = directory.filePath(filename);
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

            if (item->state() == DownloadItemViewModel::Done)
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
            QSharedPointer<DownloadItemViewModel> item(new DownloadItemViewModel(m_dialog.data()));

            if (!item->deserialize(doc, e))
                continue;

            QObject::connect(item.data(), SIGNAL(propertyChanged(ComponentModel::PropertyChangedSignalArgs)), this, SLOT(propertyChanged(ComponentModel::PropertyChangedSignalArgs)));
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


QAction* MainViewModel::smartModeAction()
{
    return &m_smartModeAction;
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
        if (state == DownloadItemViewModel::Download || state == DownloadItemViewModel::Convert || state == DownloadItemViewModel::Pause)
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

        // Show settings

        QSettings settings;
        if (settings.value("Download/smartMode", false).toBool())
        {
            DownloadSettingsViewModel * downloadSettings = new DownloadSettingsViewModel(m_dialog.data(), url, isPlaylist);
            QSharedPointer<DownloadItemViewModel> item(new DownloadItemViewModel(m_dialog.data(), downloadSettings));
            QObject::connect(item.data(), SIGNAL(propertyChanged(ComponentModel::PropertyChangedSignalArgs)),
                             this, SLOT(propertyChanged(ComponentModel::PropertyChangedSignalArgs)));
            QObject::connect(item.data(), SIGNAL(parsed(ViewModel::MediaDownloadList)),
                             this, SLOT(itemParsed(ViewModel::MediaDownloadList)));
            QObject::connect(item.data(), SIGNAL(downloadCompleted()),
                             this, SLOT(onItemDownloadCompleted()));
            m_list.insert(0, item);
        }
        else
        {
            QScopedPointer<DownloadSettingsViewModel> downloadSettings(new DownloadSettingsViewModel(m_dialog.data(), url, isPlaylist));
            if (QDialog::Accepted == m_dialog.data()->showDialog(downloadSettings.data()))
            {
                // Start download

                MediaDownloadList downloadList = downloadSettings->downloadList();
                for (int i = 0; i < downloadList.count(); ++i)
                {
                    QSharedPointer<DownloadItemViewModel> item(new DownloadItemViewModel(m_dialog.data(), downloadList[i]));
                    QObject::connect(item.data(), SIGNAL(propertyChanged(ComponentModel::PropertyChangedSignalArgs)),
                                     this, SLOT(propertyChanged(ComponentModel::PropertyChangedSignalArgs)));
                    QObject::connect(item.data(), SIGNAL(downloadCompleted()),
                                     this, SLOT(onItemDownloadCompleted()));
                    m_list.insert(i, item);
                }
            }
            else if (downloadSettings->state() == DownloadSettingsViewModel::Error)
            {
                // Post failed link

                QList<Mvvm::Dialog::MessageButton> buttons;
                buttons << Mvvm::Dialog::MessageButton(tr("Post"), QMessageBox::AcceptRole);
                buttons << Mvvm::Dialog::MessageButton(tr("Close"), QMessageBox::RejectRole);

                int result = m_dialog.data()->showMessageDialog(tr("Can't parse link. Please post it to our support and we'll check it."),
                                                                QMessageBox::Information, QCoreApplication::applicationName(), buttons, 0);
            }
        }
    }
    catch (...)
    {
        QList<Mvvm::Dialog::MessageButton> buttons;
        buttons << Mvvm::Dialog::MessageButton(tr("Close"), QMessageBox::RejectRole);

        m_dialog.data()->showMessageDialog(tr("No valid url found in the clipboard. Please copy video link from the browser address bar and press \"Paste URL\" again."),
                                           QMessageBox::Information, QCoreApplication::applicationName(), buttons, 0);
    }
}


void MainViewModel::showSmartMode()
{
    QScopedPointer<SmartModeViewModel> smartModeSettings(new SmartModeViewModel(m_dialog.data()));
    m_dialog.data()->showDialog(smartModeSettings.data());

    updateSmartMode();
}


void MainViewModel::showInformation()
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


void MainViewModel::updateSmartMode()
{
    QSettings settings;
    if (settings.value("Download/smartMode", false).toBool())
        m_smartModeAction.setIcon(QIcon(":/image/smart-mode-on"));
    else
        m_smartModeAction.setIcon(QIcon(":/image/smart-mode-off"));
}

void MainViewModel::propertyChanged(const ComponentModel::PropertyChangedSignalArgs& args)
{
}


void MainViewModel::listChanged(const ListChangedSignalArgs& args)
{
    Q_UNUSED(args)
    updateActions(m_selection.currentIndex());
}


void MainViewModel::selectionChanged(const SelectionChangedSignalArgs& args)
{
    updateActions(args.currentIndex());
}


void MainViewModel::itemParsed(ViewModel::MediaDownloadList downloadList)
{
    int index = m_list.indexOf(sender());

    for (int i = index + 1; i < downloadList.count(); ++i)
    {
        QSharedPointer<DownloadItemViewModel> item(new DownloadItemViewModel(m_dialog.data(), downloadList[i]));
        QObject::connect(item.data(), SIGNAL(propertyChanged(ComponentModel::PropertyChangedSignalArgs)),
                         this, SLOT(propertyChanged(ComponentModel::PropertyChangedSignalArgs)));
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
        case DownloadItemViewModel::Download:
        case DownloadItemViewModel::Convert:
            return;

        default:
            break;
        }
    }

    emit downloadCompleted();
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


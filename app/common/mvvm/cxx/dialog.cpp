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


#include "mvvm/cxx/dialog.h"
#include <QInputDialog>
#include <QSettings>
#include <QDesktopServices>
#include <QApplication>

using namespace Mvvm;


static QString openDialogDirectory;
static QString saveDialogDirectory;
static QString dirDialogDirectory;


Dialog::Dialog(const Factory* factory, QObject *parent) :
    QObject(parent),
    m_factory(factory)
{
    qRegisterMetaType<MultiSelectMode>();

    QSettings settings;
    QVariant defaultDirectory = QVariant::fromValue(QDesktopServices::storageLocation(QDesktopServices::MoviesLocation));
    openDialogDirectory = settings.value("Dialog/openDialogDirectory", defaultDirectory).toString();
    saveDialogDirectory = settings.value("Dialog/saveDialogDirectory", defaultDirectory).toString();
    dirDialogDirectory = settings.value("Dialog/dirDialogDirectory", QVariant::fromValue(QString())).toString();
}


Dialog::~Dialog()
{
    QSettings settings;
    if (!openDialogDirectory.isEmpty())
        settings.setValue("Dialog/openDialogDirectory", openDialogDirectory);
    if (!saveDialogDirectory.isEmpty())
        settings.setValue("Dialog/saveDialogDirectory", saveDialogDirectory);
    if (!dirDialogDirectory.isEmpty())
        settings.setValue("Dialog/dirDialogDirectory", dirDialogDirectory);
}


int Dialog::showDialog(QObject* viewModel, bool modal) const
{
    if (m_factory.isNull())
        return QDialog::Rejected;

    QScopedPointer<QObject> view(m_factory.data()->createView(viewModel));
    QDialog* dialog = qobject_cast<QDialog*>(view.data());

    if (dialog != NULL)
    {
        if (modal)
            return dialog->exec();
        else
        {
            view.take();
            dialog->setAttribute(Qt::WA_DeleteOnClose);
            dialog->show();
            return QDialog::Accepted;
        }
    }

    return QDialog::Rejected;
}


int Dialog::showOpenDialog(QStringList& selected, MultiSelectMode mode,
                           const QString& filters, QString* selectedFilter,
                           const QString& directory, const QString& caption,
                           QFileDialog::Options options) const
{
    if (mode == SingleSelect)
    {
        QString result = QFileDialog::getOpenFileName(QApplication::activeWindow(), caption, directory.isEmpty() ? openDialogDirectory : directory,
                                                      filters, selectedFilter, options);
        selected = result.isEmpty() ? QStringList() : QStringList() << result;
    }
    else
    {
        selected = QFileDialog::getOpenFileNames(QApplication::activeWindow(), caption, directory.isEmpty() ? openDialogDirectory : directory,
                                                 filters, selectedFilter, options);
    }

    if (!selected.isEmpty())
    {
        openDialogDirectory = QFileInfo(selected.at(0)).canonicalPath();
        return QDialog::Accepted;
    }

    return QDialog::Rejected;
}


int Dialog::showSaveDialog(QString& selected, const QString& filters, QString* selectedFilter,
                           const QString& directory, const QString& caption,
                           QFileDialog::Options options) const
{
    selected = QFileDialog::getSaveFileName(QApplication::activeWindow(), caption, directory.isEmpty() ? saveDialogDirectory : directory,
                                            filters, selectedFilter, options);

    if (!selected.isEmpty())
    {
        saveDialogDirectory = QFileInfo(selected).canonicalPath();
        return QDialog::Accepted;
    }

    return QDialog::Rejected;
}


int Dialog::showDirectoryDialog(QString& selected, const QString& directory, const QString& caption,
                                QFileDialog::Options options) const
{
    selected = QFileDialog::getExistingDirectory(QApplication::activeWindow(), caption, directory.isEmpty() ? dirDialogDirectory : directory,
                                                 options);

    if (!selected.isEmpty())
    {
        dirDialogDirectory = QDir(selected).canonicalPath();
        return QDialog::Accepted;
    }

    return QDialog::Rejected;
}


int Dialog::showInputDialog(QString& result, const QString& caption, const QString& label, const QString& text) const
{
    bool ok = false;
    result = QInputDialog::getText(QApplication::activeWindow(), caption, label, QLineEdit::Normal, text, &ok);

    if (ok && !result.isEmpty())
        return QDialog::Accepted;

    return QDialog::Rejected;
}


int Dialog::showMessageDialog(const QString& text, QMessageBox::Icon icon, const QString& caption, const QList<MessageButton>& buttons, int defaultIndex) const
{
    QMessageBox messageBox(icon, caption, text, QMessageBox::NoButton, QApplication::activeWindow());
    QList<QAbstractButton*> buttonList;

    for (int i = 0; i < buttons.count(); ++i)
    {
        buttonList.append(
            (QAbstractButton*)messageBox.addButton(buttons[i].first, buttons[i].second)
        );
    }

    if (defaultIndex >= 0 && defaultIndex < buttonList.count())
        messageBox.setDefaultButton((QPushButton*)buttonList[defaultIndex]);

    messageBox.exec();
    return buttonList.indexOf(messageBox.clickedButton());
}

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


#ifndef MVVM_CXX_DIALOG_H
#define MVVM_CXX_DIALOG_H

#include <QObject>
#include <QFileDialog>
#include <QMessageBox>
#include <QMetaType>
#include "mvvm/cxx/factory.h"
#include "mvvm/cxx/metatypes.h"

namespace Mvvm
{

class Dialog : public QObject
{
    Q_OBJECT
    Q_ENUMS(MultiSelectMode)

public:
    enum MultiSelectMode { SingleSelect, MultiSelect };
    typedef QPair<QString, QMessageBox::ButtonRole> MessageButton;

    Dialog(const Mvvm::Factory* factory, QObject* parent = 0);
    virtual ~Dialog();

    int showDialog(QObject* viewModel, bool modal = true) const;
    int showOpenDialog(QStringList& selected, Mvvm::Dialog::MultiSelectMode multiselect = Mvvm::Dialog::SingleSelect,
                       const QString& filters = QString(), QString* selectedFilter = NULL,
                       const QString& directory = QString(), const QString& caption = QString(),
                       QFileDialog::Options options = QFlags<QFileDialog::Option>()) const;
    int showSaveDialog(QString& selected, const QString& filters = QString(), QString* selectedFilter = NULL,
                       const QString& directory = QString(), const QString& caption = QString(),
                       QFileDialog::Options options = QFlags<QFileDialog::Option>()) const;
    int showDirectoryDialog(QString& selected, const QString& directory = QString(), const QString& caption = QString(),
                            QFileDialog::Options options = QFileDialog::ShowDirsOnly) const;
    int showInputDialog(QString& result, const QString& caption, const QString& label, const QString& text = QString()) const;
    int showMessageDialog(const QString& text, QMessageBox::Icon icon = QMessageBox::NoIcon, const QString& caption = QString(),
                          const QList<MessageButton>& buttons = QList<MessageButton>(), int defaultIndex = -1) const;

private:
    QWeakPointer<const Mvvm::Factory> m_factory;
    mutable QByteArray m_fileDialogState;
};

} // Mvvm

Q_DECLARE_METATYPE(Mvvm::Dialog::MultiSelectMode)

#endif // MVVM_CXX_DIALOG_H

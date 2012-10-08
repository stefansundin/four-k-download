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


#ifndef MVVM_QML_DIALOG_H
#define MVVM_QML_DIALOG_H

#include <QObject>
#include <QStringList>
#include <QMetaType>
#include <QDeclarativeView>
#include "mvvm/qml/metatypes.h"


namespace Mvvm
{

class Dialog : public QObject
{
    Q_OBJECT
    Q_ENUMS(IconType DialogType)

public:
    enum IconType { NoIcon = 0, InformationIcon = 1, WarningIcon = 2, CriticalIcon = 3, QuestionIcon = 4 };
    enum DialogType { ModalDialog, FloatingDialog, MainWindow };

    explicit Dialog(QObject *parent = 0);
    virtual ~Dialog();

    Q_INVOKABLE int showDialog(const QString& view, int type = ModalDialog);
    Q_INVOKABLE int showDialog(const QString& view, QObject* viewModel, int type = ModalDialog);

    Q_INVOKABLE QStringList showOpenDialog(bool singleSelect = true, const QString& filters = QString(),
                                           const QString& directory = QString(), const QString& caption = QString()) const;
    Q_INVOKABLE QString showSaveDialog(const QString& filters = QString(), const QString& directory = QString(),
                                       const QString& caption = QString()) const;
    Q_INVOKABLE QString showDirectoryDialog(const QString& directory = QString(), const QString& caption = QString()) const;
    Q_INVOKABLE int showMessageDialog(const QString& text, int icon = NoIcon,
                                      const QString& caption = QString(),
                                      const QStringList& buttons = QStringList(),
                                      int defaultIndex = -1) const;

public:
    static void addImportPath(QString path);
    static void addPluginPath(QString path);
};


class DialogCallbacks : public QObject
{
    Q_OBJECT

public:
    DialogCallbacks(QWidget* dialog, QDeclarativeView* declarative, QObject* view);

protected slots:
    void viewFullScreenChanged();
    void widthChanged();
    void heightChanged();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    QWidget* m_dialog;
    QDeclarativeView* m_declarative;
    QObject* m_view;
};

} // Mvvm

Q_DECLARE_METATYPE(Mvvm::Dialog::IconType)
Q_DECLARE_METATYPE(Mvvm::Dialog::DialogType)

#endif // MVVM_QML_DIALOG_H

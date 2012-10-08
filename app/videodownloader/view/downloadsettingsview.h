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


#ifndef DOWNLOADSETTINGSVIEW_H
#define DOWNLOADSETTINGSVIEW_H

#include <QWidget>
#include <QMovie>
#include <QMenu>
#include "viewmodel/downloadsettingsviewmodel.h"
#include "componentmodel/propertybinding.h"
#include "gui/cxx/buttonactionbinding.h"
#include "gui/cxx/comboboxbinding.h"
#include "mvvm/cxx/factory.h"

namespace Ui
{
    class DownloadSettingsView;
}

namespace View
{

class DownloadSettingsView : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(QObject* viewModel READ viewModel WRITE setViewModel)

public:
    Q_INVOKABLE DownloadSettingsView(const Mvvm::Factory* factory, QWidget *parent = 0);
    virtual ~DownloadSettingsView();

    QObject* viewModel() const;
    void setViewModel(QObject* value);

protected slots:
    void propertyChanged(const ComponentModel::PropertyChangedSignalArgs& args);
    void radioButtonToggled(bool toggled);
    void saveEditingFinished();
    void buttonBoxClicked(QAbstractButton* button);
    void contextMenuRequest(const QPoint& point);

private:
    Ui::DownloadSettingsView *ui;
    QMovie* m_movie;
    QPushButton* m_acceptButton;
    QWeakPointer<const Mvvm::Factory> m_factory;
    QWeakPointer<ViewModel::DownloadSettingsViewModel> m_viewModel;
    QMenu m_menu;
    QScopedPointer<ComponentModel::PropertyBinding> m_subtitlesEnabled1Binding;
    QScopedPointer<ComponentModel::PropertyBinding> m_subtitlesEnabled2Binding;
    QScopedPointer<ComponentModel::PropertyBinding> m_subtitlesDownloadBinding;
    QScopedPointer<Gui::ComboboxBinding> m_subtitlesBinding;
    QScopedPointer<ComponentModel::PropertyBinding> m_fileBinding;
    QScopedPointer<Gui::ButtonActionBinding> m_fileActionBinding;
};

} // View

#endif // DOWNLOADSETTINGSVIEW_H

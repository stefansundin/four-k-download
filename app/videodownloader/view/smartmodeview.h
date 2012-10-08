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


#ifndef SMARTMODEVIEW_H
#define SMARTMODEVIEW_H

#include <QDialog>
#include "viewmodel/smartmodeviewmodel.h"
#include "componentmodel/propertybinding.h"
#include "gui/cxx/buttonactionbinding.h"
#include "gui/cxx/comboboxbinding.h"
#include "mvvm/cxx/factory.h"

namespace Ui
{
    class SmartModeView;
}

namespace View
{

class SmartModeView : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(QObject* viewModel READ viewModel WRITE setViewModel)

public:
    Q_INVOKABLE SmartModeView(const Mvvm::Factory* factory, QWidget *parent = 0);
    virtual ~SmartModeView();

    QObject* viewModel() const;
    void setViewModel(QObject* value);

protected slots:
    void buttonBoxClicked(QAbstractButton* button);

private:
    Ui::SmartModeView *ui;
    QWeakPointer<const Mvvm::Factory> m_factory;
    QWeakPointer<ViewModel::SmartModeViewModel> m_viewModel;
    QScopedPointer<ComponentModel::PropertyBinding> m_smartModeBinding;
    QScopedPointer<ComponentModel::PropertyBinding> m_settingsEnabledBinding;
    QScopedPointer<ComponentModel::PropertyBinding> m_qualityEnabledBinding1;
    QScopedPointer<ComponentModel::PropertyBinding> m_qualityEnabledBinding2;
    QScopedPointer<Gui::ComboboxBinding> m_formatBinding;
    QScopedPointer<Gui::ComboboxBinding> m_qualityBinding;
    QScopedPointer<ComponentModel::PropertyBinding> m_dirBinding;
    QScopedPointer<Gui::ButtonActionBinding> m_dirButtonBinding;
};

} // View

#endif // SMARTMODEVIEW_H

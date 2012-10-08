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


#ifndef MEDIAITEMVIEW_H
#define MEDIAITEMVIEW_H

#include <QWidget>
#include "viewmodel/mediaitemviewmodel.h"
#include "componentmodel/propertybinding.h"


namespace Ui
{
    class MediaItemView;
}

namespace View
{

class MediaItemView : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QObject* viewModel READ viewModel WRITE setViewModel)

public:
    Q_INVOKABLE MediaItemView(const Mvvm::Factory* factory, QWidget* parent = 0);
    virtual ~MediaItemView();

    QObject* viewModel() const;
    void setViewModel(QObject* value);
    void setCheckedVisible(bool visible);
    void setOdd(bool odd);

protected:
    void updateState();

protected slots:
    void propertyChanged(const ComponentModel::PropertyChangedSignalArgs& args);

private:
    Ui::MediaItemView *ui;
    QWeakPointer<const Mvvm::Factory> m_factory;
    QWeakPointer<ViewModel::MediaItemViewModel> m_viewModel;
    QScopedPointer<ComponentModel::PropertyBinding> m_thumbnailBinding;
    QScopedPointer<ComponentModel::PropertyBinding> m_titleBinding;
    QScopedPointer<ComponentModel::PropertyBinding> m_durationBinding;
    QScopedPointer<ComponentModel::PropertyBinding> m_urlBinding;
    QScopedPointer<ComponentModel::PropertyBinding> m_checkedBinding;
};

} // View

#endif // MEDIAITEMVIEW_H

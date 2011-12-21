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


#ifndef MEDIADETAILSVIEW_H
#define MEDIADETAILSVIEW_H

#include <QWidget>
#include "viewmodel/mediadetailsviewmodel.h"

namespace Ui
{
    class MediaDetailsView;
}

namespace View
{

class MediaDetailsView : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QObject* viewModel READ viewModel WRITE setViewModel)

public:
    Q_INVOKABLE MediaDetailsView(const Mvvm::Factory* factory, QWidget* parent = 0);
    virtual ~MediaDetailsView();

    QObject* viewModel() const;
    void setViewModel(QObject* value);
    bool isChecked() const;
    void setChecked(bool value);

signals:
    void toggled(bool checked);

protected slots:
    void radioButtonClicked();
    void radioButtonToggled(bool checked);

protected:
    void mousePressEvent(QMouseEvent* event);

private:
    Ui::MediaDetailsView *ui;
    QWeakPointer<const Mvvm::Factory> m_factory;
    QWeakPointer<ViewModel::MediaDetailsViewModel> m_viewModel;
};

} // View

#endif // MEDIADETAILSVIEW_H

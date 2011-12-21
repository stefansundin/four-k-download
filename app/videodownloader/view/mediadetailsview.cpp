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


#include "view/mediadetailsview.h"
#include "ui_mediadetailsview.h"

using namespace View;
using namespace ViewModel;


MediaDetailsView::MediaDetailsView(const Mvvm::Factory* factory, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MediaDetailsView),
    m_factory(factory)
{
    ui->setupUi(this);

    QObject::connect(ui->summaryButton, SIGNAL(clicked()), this, SLOT(radioButtonClicked()));
    QObject::connect(ui->summaryButton, SIGNAL(toggled(bool)), this, SLOT(radioButtonToggled(bool)));
}


MediaDetailsView::~MediaDetailsView()
{
    delete ui;
}


QObject* MediaDetailsView::viewModel() const
{
    return m_viewModel.data();
}


void MediaDetailsView::setViewModel(QObject* value)
{
    if (m_viewModel.data() != value)
    {
        m_viewModel = dynamic_cast<MediaDetailsViewModel*>(value);

        if (m_viewModel)
        {
            ui->summaryButton->setText(m_viewModel.data()->summary());
            ui->videoLabel->setText(m_viewModel.data()->quality());
            ui->codecsLabel->setText(m_viewModel.data()->codecs());
            ui->sizeLabel->setText(m_viewModel.data()->fileSize());
        }
    }
}


bool MediaDetailsView::isChecked() const
{
    return ui->summaryButton->isChecked();
}


void MediaDetailsView::setChecked(bool value)
{
    ui->summaryButton->setChecked(value);
}


void MediaDetailsView::radioButtonClicked()
{
    ui->summaryButton->setChecked(true);
}


void MediaDetailsView::radioButtonToggled(bool checked)
{
    emit toggled(checked);
}


void MediaDetailsView::mousePressEvent(QMouseEvent* event)
{
    ui->summaryButton->setChecked(true);
}

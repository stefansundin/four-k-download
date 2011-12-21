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


#include "view/mediaitemview.h"
#include "ui_mediaitemview.h"
#include "gui/binding/elidedtextconverter.h"

using namespace View;
using namespace ViewModel;
using namespace Bindings;


MediaItemView::MediaItemView(const Mvvm::Factory* factory, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MediaItemView),
    m_factory(factory)
{
    ui->setupUi(this);

    QFont font = ui->titleLabel->font();
    font.setBold(true);
    font.setPointSize(font.pointSize() + 2);
    ui->titleLabel->setFont(font);

    ui->durationIconLabel->setVisible(false);
    ui->durationLabel->setVisible(false);
    ui->urlIconLabel->setVisible(false);
    ui->urlLabel->setVisible(false);
    ui->parsingIconLabel->setVisible(true);
    ui->parsingLabel->setVisible(true);
    ui->errorIconLabel->setVisible(false);
    ui->errorLabel->setVisible(false);
	
#if defined(Q_OS_MAC)
	ui->checkedBox->setText(" ");
#endif
}


MediaItemView::~MediaItemView()
{
    delete ui;
}


QObject* MediaItemView::viewModel() const
{
    return m_viewModel.data();
}


void MediaItemView::setViewModel(QObject* value)
{
    if (m_viewModel.data() != value)
    {
        if (m_viewModel)
        {
            m_thumbnailBinding.reset();
            m_titleBinding.reset();
            m_durationBinding.reset();
            m_urlBinding.reset();
            m_checkedBinding.reset();

            QObject::disconnect(this, SLOT(propertyChanged(ComponentModel::PropertyChangedSignalArgs)));
        }

        m_viewModel = dynamic_cast<MediaItemViewModel*>(value);

        if (m_viewModel)
        {
            m_thumbnailBinding.reset(new Binding(ui->thumbnailLabel, "pixmap", m_viewModel.data(), "thumbnail"));
            m_titleBinding.reset(new Binding(ui->titleLabel, "text", m_viewModel.data(), "title"));
            m_durationBinding.reset(new Binding(ui->durationLabel, "text", m_viewModel.data(), "duration"));
            m_urlBinding.reset(new Binding(ui->urlLabel, "text", m_viewModel.data(), "url"));
            m_checkedBinding.reset(new Binding(ui->checkedBox, "checked", m_viewModel.data(), "isChecked", Binding::TwoWay));

            QObject::connect(m_viewModel.data(), SIGNAL(propertyChanged(ComponentModel::PropertyChangedSignalArgs)),
                             this, SLOT(propertyChanged(ComponentModel::PropertyChangedSignalArgs)));
        }

        updateState();
    }
}


void MediaItemView::setCheckedVisible(bool visible)
{
    if (!visible)
    {
        ui->checkedBox->setVisible(visible);
        ui->titleLabel->setIndent(0);
    }
}


void MediaItemView::setOdd(bool odd)
{
    if (odd)
        setStyleSheet("background-color: rgb(246, 246, 246);");
    else
        setStyleSheet("background-color: rgb(255, 255, 255);");
}


void MediaItemView::updateState()
{
    if (!m_viewModel)
        return;

    MediaItemViewModel::State state = m_viewModel.data()->state();

    ui->durationIconLabel->setVisible(state == MediaItemViewModel::Done);
    ui->durationLabel->setVisible(state == MediaItemViewModel::Done);
    ui->urlIconLabel->setVisible(state == MediaItemViewModel::Done);
    ui->urlLabel->setVisible(state == MediaItemViewModel::Done);
    ui->parsingIconLabel->setVisible(state == MediaItemViewModel::Parsing);
    ui->parsingLabel->setVisible(state == MediaItemViewModel::Parsing);
    ui->errorIconLabel->setVisible(state == MediaItemViewModel::Error);
    ui->errorLabel->setVisible(state == MediaItemViewModel::Error);

    if (state == MediaItemViewModel::Error)
        ui->checkedBox->setEnabled(false);
}


void MediaItemView::propertyChanged(const ComponentModel::PropertyChangedSignalArgs& args)
{
    if (args.property() == "state")
    {
        updateState();
    }
}

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


#include "smartmodeview.h"
#include "ui_smartmodeview.h"

using namespace View;
using namespace ViewModel;
using namespace ComponentModel;
using namespace Gui;
using namespace Mvvm;


SmartModeView::SmartModeView(const Factory* factory, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SmartModeView),
    m_factory(factory)
{
    ui->setupUi(this);

    ui->buttonBox->addButton(tr("Cancel"), QDialogButtonBox::RejectRole);
    ui->buttonBox->addButton(tr("Ok"), QDialogButtonBox::AcceptRole);

    QColor color = ui->line->palette().color(QPalette::Window).darker(112);
    QString stylesheet = QString("background-color: white; color: rgb(%1, %2, %3);").arg(
                QString::number(color.red()),
                QString::number(color.green()),
                QString::number(color.blue())
                );
    ui->line->setStyleSheet(stylesheet);

    QObject::connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)),
                     this, SLOT(buttonBoxClicked(QAbstractButton*)));

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

SmartModeView::~SmartModeView()
{
    delete ui;
}


QObject* SmartModeView::viewModel() const
{
    return m_viewModel.data();
}


void SmartModeView::setViewModel(QObject* value)
{
    if (m_viewModel.data() != value)
    {
        if (m_viewModel)
        {
            m_smartModeBinding.reset();
            m_settingsEnabledBinding.reset();
            m_qualityEnabledBinding1.reset();
            m_qualityEnabledBinding2.reset();
            m_formatBinding.reset();
            m_qualityBinding.reset();
            m_dirBinding.reset();
            m_dirButtonBinding.reset();
        }


        m_viewModel = dynamic_cast<SmartModeViewModel*>(value);


        if (m_viewModel && m_factory)
        {
            m_smartModeBinding.reset(new PropertyBinding(ui->smartModeBox, "checked", m_viewModel.data(), "smartMode", PropertyBinding::TwoWay));
            m_settingsEnabledBinding.reset(new PropertyBinding(ui->settingsWidget, "enabled", m_viewModel.data(), "smartMode"));
            m_qualityEnabledBinding1.reset(new PropertyBinding(ui->qualityBox, "enabled", m_viewModel.data(), "qualityEnabled"));
            m_qualityEnabledBinding2.reset(new PropertyBinding(ui->qualityLabel, "enabled", m_viewModel.data(), "qualityEnabled"));
            m_formatBinding.reset(new ComboboxBinding(ui->formatBox, m_viewModel.data(), "formatList", "formatIndex"));
            m_qualityBinding.reset(new ComboboxBinding(ui->qualityBox, m_viewModel.data(), "qualityList", "qualityIndex"));
            m_dirBinding.reset(new PropertyBinding(ui->directoryBox, "text", m_viewModel.data(), "dirName"));
            m_dirButtonBinding.reset(new ButtonActionBinding(ui->directoryButton, m_viewModel.data()->dirAction()));
        }
    }
}


void SmartModeView::buttonBoxClicked(QAbstractButton* button)
{
    if (ui->buttonBox->buttonRole(button) == QDialogButtonBox::AcceptRole)
    {
        m_viewModel.data()->saveState();
        accept();
    }
    else
        reject();
}

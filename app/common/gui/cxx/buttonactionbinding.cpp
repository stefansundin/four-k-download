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


#include "gui/cxx/buttonactionbinding.h"
#include <QDebug>

using namespace Gui;


ButtonActionBinding::ButtonActionBinding(QAbstractButton* button, QAction* action, BindOptions options, QObject* parent) :
    QObject(parent),
    m_button(NULL),
    m_action(NULL),
    m_options(options)
{
    Q_ASSERT_X(button != NULL && action != NULL, "ButtonActionBinding", "object parameters must not be null.");

    m_button = button;
    m_action = action;

    m_button->addAction(m_action);

    QObject::connect(m_button, SIGNAL(clicked(bool)), this, SLOT(buttonClicked(bool)));
    QObject::connect(m_button, SIGNAL(destroyed()), this, SLOT(reset()));
    QObject::connect(m_action, SIGNAL(changed()), this, SLOT(actionChanged()));
    QObject::connect(m_action, SIGNAL(destroyed()), this, SLOT(reset()));

    actionChanged();
}


ButtonActionBinding::~ButtonActionBinding()
{
    reset();
}


void ButtonActionBinding::buttonClicked(bool checked)
{
    m_action->setChecked(checked);
    m_action->trigger();
}


void ButtonActionBinding::actionChanged()
{
    if (!m_options.testFlag(notBindVisibility))
        m_button->setVisible(m_action->isVisible());
    m_button->setEnabled(m_action->isEnabled());
    m_button->setCheckable(m_action->isCheckable());
    m_button->setChecked(m_action->isChecked());
    if (!m_options.testFlag(notBindText))
        m_button->setText(m_action->text());
    m_button->setToolTip(m_action->toolTip());
    if (!m_options.testFlag(notBindIcon))
        m_button->setIcon(m_action->icon());
}


void ButtonActionBinding::reset()
{
    if (m_button == NULL || m_action == NULL)
        return;

    QObject::disconnect(this, SLOT(actionChanged()));
    QObject::disconnect(this, SLOT(buttonClicked(bool)));
    QObject::disconnect(this, SLOT(reset()));

    m_button->removeAction(m_action);

    m_action = NULL;
    m_button = NULL;
}

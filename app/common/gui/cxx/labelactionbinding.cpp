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


#include "gui/cxx/labelactionbinding.h"
#include <QDebug>

using namespace Gui;


LabelActionBinding::LabelActionBinding(QLabel* label, QAction* action, BindOptions options, QObject* parent) :
    QObject(parent),
    m_label(NULL),
    m_action(NULL),
    m_options(options)
{
    Q_ASSERT_X(label != NULL && action != NULL, "LabelActionBinding", "object parameters must not be null.");

    m_label = label;
    m_action = action;

    m_label->addAction(m_action);

    QObject::connect(m_label, SIGNAL(linkActivated(QString)), this, SLOT(labelClicked()));
    QObject::connect(m_label, SIGNAL(destroyed()), this, SLOT(reset()));
    QObject::connect(m_action, SIGNAL(changed()), this, SLOT(actionChanged()));
    QObject::connect(m_action, SIGNAL(destroyed()), this, SLOT(reset()));

    actionChanged();
}


LabelActionBinding::~LabelActionBinding()
{
    reset();
}


void LabelActionBinding::labelClicked()
{
    m_action->trigger();
}


void LabelActionBinding::actionChanged()
{
    if (!m_options.testFlag(notBindVisibility))
        m_label->setVisible(m_action->isVisible());
    m_label->setEnabled(m_action->isEnabled());
    m_label->setToolTip(m_action->toolTip());

    QString text = "<a href=\"ref\">%1</a>";
    m_label->setText(text.arg(m_action->text()));
}


void LabelActionBinding::reset()
{
    if (m_label == NULL || m_action == NULL)
        return;

    QObject::disconnect(this, SLOT(actionChanged()));
    QObject::disconnect(this, SLOT(labelClicked()));
    QObject::disconnect(this, SLOT(reset()));

    m_label->removeAction(m_action);

    m_action = NULL;
    m_label = NULL;
}

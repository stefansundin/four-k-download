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


#include "gui/cxx/comboboxbinding.h"
#include "componentmodel/signalblocker.h"
#include <QDebug>
#include <QMetaProperty>

using namespace ComponentModel;
using namespace Gui;


Q_DECLARE_METATYPE(QList<QIcon>)


ComboboxBinding::ComboboxBinding(QComboBox* combobox, QObject* srcObject,
                                 const QString& itemsProperty, const QString& indexProperty,
                                 const QString& iconsProperty, QObject *parent) :
    QObject(parent),
    m_combobox(NULL),
    m_srcObject(NULL)
{
    Q_ASSERT_X(combobox != NULL && srcObject != NULL, "ComboboxBinding", "object parameters must not be null.");

    m_combobox = combobox;
    m_srcObject = srcObject;
    m_itemsProperty = itemsProperty;
    m_iconsProperty = iconsProperty;    

    m_itemsBinding.reset(new PropertyBinding(this, "items", m_srcObject, m_itemsProperty));
    if (!m_iconsProperty.isEmpty())
        m_iconsBinding.reset(new PropertyBinding(this, "icons", m_srcObject, m_iconsProperty));
    m_indexBinding.reset(new PropertyBinding(m_combobox, "currentIndex", m_srcObject, indexProperty, PropertyBinding::TwoWay));
}


QStringList ComboboxBinding::items() const
{
    return QStringList();
}


void ComboboxBinding::setItems(QStringList value)
{
    updateItems();
}


QList<QIcon> ComboboxBinding::icons() const
{
    return QList<QIcon>();
}


void ComboboxBinding::setIcons(QList<QIcon> value)
{
    updateItems();
}


bool ComboboxBinding::updateItems() const
{
    SignalBlocker blocker(m_combobox); Q_UNUSED(blocker)

    m_combobox->clear();

    if (m_iconsProperty.isEmpty())
    {
        m_combobox->addItems(m_srcObject->property(m_itemsProperty.toAscii()).toStringList());
    }
    else
    {
        QStringList items = m_srcObject->property(m_itemsProperty.toAscii()).toStringList();
        QList<QIcon> icons = m_srcObject->property(m_iconsProperty.toAscii()).value< QList<QIcon> >();

        int itemsCount = items.count();
        int iconsCount = icons.count();
        for(int i = 0; i < itemsCount; ++i)
            m_combobox->addItem((i < iconsCount) ? icons.at(i) : QIcon(), items.at(i));
    }

    return true;
}

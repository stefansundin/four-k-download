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


#include "componentmodel/propertybinding.h"
#include "componentmodel/signalblocker.h"
#include <QDebug>
#include <QMetaProperty>

using namespace ComponentModel;


PropertyBinding::PropertyBinding(QObject* dstObject, const QString& dstProperty,
                 QObject* srcObject, const QString& srcProperty,
                 Mode mode, PropertyValueConverter* converter, QObject* parent) :
    QObject(parent),
    m_srcObject(NULL),
    m_dstObject(NULL),
    m_mode(mode),
    m_converter(converter)
{
    Q_ASSERT_X(srcObject != NULL && dstObject != NULL, "PropertyBinding", "object parameters must not be null.");


    int srcIndex = srcObject->metaObject()->indexOfProperty(srcProperty.toAscii());
    int dstIndex = dstObject->metaObject()->indexOfProperty(dstProperty.toAscii());

    Q_ASSERT_X(srcIndex != -1 && dstIndex != -1, "PropertyBinding",
               QString("property \"%1\" or \"%2\" is missing.").arg(srcProperty, dstProperty).toAscii());


    QMetaProperty srcMetaProperty = srcObject->metaObject()->property(srcIndex);
    QMetaProperty dstMetaProperty = dstObject->metaObject()->property(dstIndex);

    if (m_mode == PropertyBinding::TwoWay || m_mode == PropertyBinding::OneWay)
    {
        Q_ASSERT_X(srcMetaProperty.isReadable() && dstMetaProperty.isWritable(), "PropertyBinding",
                   QString("properties \"%1\" and \"%2\" are invalid for current mode.").arg(srcProperty, dstProperty).toAscii());
    }

    if (m_mode == PropertyBinding::TwoWay || m_mode == PropertyBinding::OneWayToSource)
    {
        Q_ASSERT_X(dstMetaProperty.isReadable() && srcMetaProperty.isWritable(), "PropertyBinding",
                   QString("properties \"%1\" and \"%2\" are invalid for current mode.").arg(srcProperty, dstProperty).toAscii());
    }


    m_srcObject = srcObject;
    m_dstObject = dstObject;
    m_srcProperty = srcProperty;
    m_dstProperty = dstProperty;

    if (m_converter.isNull())
        m_converter = new PropertyValueConverter(this);


    QObject::connect(m_srcObject, SIGNAL(destroyed()), this, SLOT(reset()));
    QObject::connect(m_dstObject, SIGNAL(destroyed()), this, SLOT(reset()));


    if (m_mode == PropertyBinding::TwoWay || m_mode == PropertyBinding::OneWay)
    {
        if (srcMetaProperty.hasNotifySignal())
        {
            QMetaObject::connect(m_srcObject, srcMetaProperty.notifySignalIndex(),
                                 this, metaObject()->indexOfSlot("propertyChanged()"));
        }
        else if (m_srcObject->inherits("ComponentModel::NotifyObject"))
        {
            QObject::connect(m_srcObject, SIGNAL(propertyChanged(ComponentModel::PropertyChangedSignalArgs)),
                             this, SLOT(propertyChanged(ComponentModel::PropertyChangedSignalArgs)));
        }

        bool updateResult = updateDestination();
        Q_ASSERT_X(updateResult, "PropertyBinding",
                   QString("Can't update destination property \"%1\"").arg(dstProperty).toAscii());
    }

    if (m_mode == PropertyBinding::TwoWay || m_mode == PropertyBinding::OneWayToSource)
    {
        if (dstMetaProperty.hasNotifySignal())
        {
            QMetaObject::connect(m_dstObject, dstMetaProperty.notifySignalIndex(),
                                 this, metaObject()->indexOfSlot("propertyChanged()"));
        }
        else if (m_dstObject->inherits("ComponentModel::NotifyObject"))
        {
            QObject::connect(m_dstObject, SIGNAL(propertyChanged(ComponentModel::PropertyChangedSignalArgs)),
                             this, SLOT(propertyChanged(ComponentModel::PropertyChangedSignalArgs)));
        }

        if (m_mode == PropertyBinding::OneWayToSource)
        {
            bool updateResult = updateSource();
            Q_ASSERT_X(updateResult, "PropertyBinding",
                       QString("Can't update source property \"%1\"").arg(srcProperty).toAscii());
        }
    }
}


PropertyBinding::~PropertyBinding()
{
    reset();
}


void PropertyBinding::propertyChanged()
{
    if (sender() == m_srcObject)
        updateDestination();
    else if (sender() == m_dstObject)
        updateSource();
}


void PropertyBinding::propertyChanged(const ComponentModel::PropertyChangedSignalArgs& args)
{
    if (args.sender() == m_srcObject && args.property() == m_srcProperty)
        updateDestination();
    else if (args.sender() == m_dstObject && args.property() == m_dstProperty)
        updateSource();
}


bool PropertyBinding::updateDestination() const
{
    if (m_mode != PropertyBinding::TwoWay && m_mode != PropertyBinding::OneWay)
        return false;

    SignalBlocker blocker(m_dstObject); Q_UNUSED(blocker)

    QVariant value = m_srcObject->property(m_srcProperty.toAscii());

    if (m_converter)
        value = m_converter.data()->convert(m_dstObject, m_dstProperty.toAscii(), value);

    if (!value.isValid())
        return false;

    m_dstObject->setProperty(m_dstProperty.toAscii(), value);
    return true;
}


bool PropertyBinding::updateSource() const
{
    if (m_mode != PropertyBinding::TwoWay && m_mode != PropertyBinding::OneWayToSource)
        return false;

    QVariant value = m_dstObject->property(m_dstProperty.toAscii());

    if (m_converter)
        value = m_converter.data()->convertBack(m_srcObject, m_srcProperty.toAscii(), value);

    if (!value.isValid())
        return false;

    m_srcObject->setProperty(m_srcProperty.toAscii(), value);
    return true;
}


void PropertyBinding::reset()
{
    if (m_srcObject == NULL || m_dstObject == NULL)
        return;

    QObject::disconnect(this, SLOT(propertyChanged()));
    QObject::disconnect(this, SLOT(propertyChanged(ComponentModel::PropertyChangedSignalArgs)));
    QObject::disconnect(this, SLOT(reset()));

    m_srcObject = NULL;
    m_dstObject = NULL;
    m_srcProperty = "";
    m_dstProperty = "";
}

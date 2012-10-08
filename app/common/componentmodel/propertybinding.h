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


#ifndef COMPONENTMODEL_PROPERTYBINDING_H
#define COMPONENTMODEL_PROPERTYBINDING_H

#include <QObject>
#include <QWeakPointer>
#include "componentmodel/propertychangedsignalargs.h"
#include "componentmodel/propertyvalueconverter.h"
#include "componentmodel/metatypes.h"

namespace ComponentModel
{

class PropertyBinding : public QObject
{
    Q_OBJECT
    Q_ENUMS(Mode)

public:
    enum Mode { TwoWay, OneWay, OneWayToSource };

    PropertyBinding(QObject* dstObject, const QString& dstProperty,
                    QObject* srcObject, const QString& srcProperty,
                    Mode mode = OneWay, PropertyValueConverter* converter = 0, 
                    QObject* parent = 0);
    virtual ~PropertyBinding();

private slots:
    void propertyChanged();
    void propertyChanged(const ComponentModel::PropertyChangedSignalArgs& args);
    void reset();

protected:
    bool updateDestination() const;
    bool updateSource() const;

private:
    QObject* m_srcObject;
    QObject* m_dstObject;
    QString m_srcProperty;
    QString m_dstProperty;
    Mode m_mode;
    QWeakPointer<PropertyValueConverter> m_converter;
};

} // ComponentModel

Q_DECLARE_METATYPE(ComponentModel::PropertyBinding::Mode)

#endif // COMPONENTMODEL_PROPERTYBINDING_H

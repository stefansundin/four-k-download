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


#include "componentmodel/notifyobject.h"
#include <stdexcept>
#include <QVariant>
#include <QMetaProperty>
using namespace ComponentModel;


NotifyObject::NotifyObject(QObject* parent) :
    QObject(parent)
{
}


void NotifyObject::emitPropertyChanged(const QString& property, const QVariant& value)
{
#ifdef QT_DEBUG
    if (this->metaObject()->indexOfProperty(property.toAscii()) == -1)
    {
        QString description = "NotifyPropertyChanged: class \"%1\" doesn't contain \"%2\" property.";
        description = description.arg(this->metaObject()->className(), property);

        throw std::invalid_argument(description.toStdString());
    }
#endif

    emit propertyChanged(PropertyChangedSignalArgs(this, property, value));
}

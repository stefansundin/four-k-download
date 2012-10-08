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


#ifndef COMPONENTMODEL_PROPERTYVALUECONVERTER_H
#define COMPONENTMODEL_PROPERTYVALUECONVERTER_H

#include <QObject>
#include <QVariant>

namespace ComponentModel
{

class PropertyValueConverter : public QObject
{
    Q_OBJECT

public:
    explicit PropertyValueConverter(QObject* parent = 0);

    virtual QVariant convert(const QObject* target, const QString& property, const QVariant& value) const;
    virtual QVariant convertBack(QObject* target, const QString& property, const QVariant& value) const;
};

} // ComponentModel

#endif // COMPONENTMODEL_PROPERTYVALUECONVERTER_H

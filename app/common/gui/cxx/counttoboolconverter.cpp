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


#include "gui/cxx/counttoboolconverter.h"
#include <QStringList>
#include <QDebug>

using namespace Gui;


CountToBoolConverter::CountToBoolConverter(int threshold, QObject *parent) :
    PropertyValueConverter(parent),
    m_threshold(threshold)
{
}


QVariant CountToBoolConverter::convert(const QObject* target, const QString& property, const QVariant& value) const
{
    Q_UNUSED(target);
    Q_UNUSED(property);

    if (value.canConvert<QStringList>())
        return value.value<QStringList>().count() >= m_threshold;
    else if (value.canConvert<int>())
        return value.value<int>() >= m_threshold;
    else
    {
        qDebug() << "CountToBoolConverter: unknown value type.";
        return QVariant();
    }
}

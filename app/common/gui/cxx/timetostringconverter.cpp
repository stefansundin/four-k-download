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


#include "gui/cxx/timetostringconverter.h"
#include "componentmodel/transform.h"
#include <QTime>
#include <QDebug>

using namespace ComponentModel;
using namespace Gui;


TimeToStringConverter::TimeToStringConverter(const QString& format, QObject* parent) :
    PropertyValueConverter(parent),
    m_format(format)
{
}


QVariant TimeToStringConverter::convert(const QObject* target, const QString& property, const QVariant& value) const
{
    Q_UNUSED(target);
    Q_UNUSED(property);

    if (value.canConvert<QTime>())
        return value.value<QTime>().toString(m_format);
    else if (value.canConvert<qint64>())
        return Transform::timeToString(value.value<qint64>(), m_format);
    else
    {
        qDebug() << "TimeToStringConverter: unknown value type.";
        return QVariant();
    }
}

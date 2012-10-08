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


#include "gui/cxx/elidedtextconverter.h"
#include <QTime>
#include <QFontMetrics>
#include <QMetaProperty>

using namespace Gui;


ElidedTextConverter::ElidedTextConverter(QObject *parent) :
    PropertyValueConverter(parent)
{
}


QVariant ElidedTextConverter::convert(const QObject* target, const QString& property, const QVariant& value) const
{
    Q_UNUSED(target);
    Q_UNUSED(property);

    int indexWidth = target->metaObject()->indexOfProperty("width");
    int indexFont = target->metaObject()->indexOfProperty("font");
    if (indexWidth != -1 && target->metaObject()->property(indexWidth).isReadable() &&
        indexFont != -1 && target->metaObject()->property(indexFont).isReadable())
    {
        int width = target->property("width").toInt();
        QFont font = target->property("font").value<QFont>();

        return QFontMetrics(font).elidedText(value.toString(), Qt::ElideRight, width);
    }

    return value;
}

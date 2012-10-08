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


#include "gui/cxx/stringformatconverter.h"
#include <QTime>

using namespace Gui;


StringFormatConverter::StringFormatConverter(const QString& format, QObject *parent) :
    PropertyValueConverter(parent),
    m_format(format)
{
}


QVariant StringFormatConverter::convert(const QObject* target, const QString& property, const QVariant& value) const
{
    Q_UNUSED(target);
    Q_UNUSED(property);

    return m_format.arg(value.toString());
}

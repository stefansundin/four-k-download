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


#ifndef GUI_CXX_ELIDEDTEXTCONVERTER_H
#define GUI_CXX_ELIDEDTEXTCONVERTER_H

#include <QObject>
#include "componentmodel/propertyvalueconverter.h"

namespace Gui
{

class ElidedTextConverter : public ComponentModel::PropertyValueConverter
{
    Q_OBJECT

public:
    explicit ElidedTextConverter(QObject* parent = 0);

    virtual QVariant convert(const QObject* target, const QString& property, const QVariant& value) const;
};

} // Gui

#endif // GUI_CXX_ELIDEDTEXTCONVERTER_H

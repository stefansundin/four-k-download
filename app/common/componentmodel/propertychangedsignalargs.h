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


#ifndef COMPONENTMODEL_PROPERTYCHANGEDSIGNALARGS_H
#define COMPONENTMODEL_PROPERTYCHANGEDSIGNALARGS_H

#include <QString>
#include <QVariant>
#include "componentmodel/signalargs.h"
#include "componentmodel/metatypes.h"

namespace ComponentModel
{

class PropertyChangedSignalArgs : public SignalArgs
{
public:
    PropertyChangedSignalArgs(QObject* sender = NULL, const QString& property = QString(""), const QVariant& value = QVariant());

    QString property() const;
    QVariant value() const;

private:
    QString m_property;
    QVariant m_value;
};

} // ComponentModel

Q_DECLARE_METATYPE(ComponentModel::PropertyChangedSignalArgs)

#endif // COMPONENTMODEL_PROPERTYCHANGEDSIGNALARGS_H

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


#ifndef COMPONENTMODEL_NOTIFYOBJECT_H
#define COMPONENTMODEL_NOTIFYOBJECT_H

#include "componentmodel/propertychangedsignalargs.h"

namespace ComponentModel
{

class NotifyObject : public QObject
{
    Q_OBJECT

public:
    explicit NotifyObject(QObject* parent = 0);

signals:
    void propertyChanged(const ComponentModel::PropertyChangedSignalArgs& args);

protected:
    void emitPropertyChanged(const QString& property, const QVariant& value = QVariant());
};

} // ComponentModel

#endif // COMPONENTMODEL_NOTIFYOBJECT_H

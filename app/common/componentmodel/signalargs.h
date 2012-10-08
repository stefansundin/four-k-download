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


#ifndef COMPONENTMODEL_SIGNALARGS_H
#define COMPONENTMODEL_SIGNALARGS_H

#include <QObject>
#include <QMetaType>
#include "componentmodel/metatypes.h"

namespace ComponentModel
{

class SignalArgs
{
public:
    explicit SignalArgs(QObject* sender = NULL);

    QObject* sender() const;

private:
    QObject* m_sender;
};

} // ComponentModel

Q_DECLARE_METATYPE(ComponentModel::SignalArgs)

#endif // COMPONENTMODEL_SIGNALARGS_H

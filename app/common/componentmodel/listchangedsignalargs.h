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


#ifndef COMPONENTMODEL_LISTCHANGEDSIGNALARGS_H
#define COMPONENTMODEL_LISTCHANGEDSIGNALARGS_H

#include <QSharedPointer>
#include "componentmodel/signalargs.h"
#include "componentmodel/metatypes.h"

namespace ComponentModel
{

class ListChangedSignalArgs : public SignalArgs
{
public:
    enum Action { None, Insert, Replace, Remove, Move, Swap, Clear };


    ListChangedSignalArgs(QObject* sender = NULL, ListChangedSignalArgs::Action action = ListChangedSignalArgs::None,
                          int index = -1, QList< QSharedPointer<QObject> > items = QList< QSharedPointer<QObject> >(),
                          int oldIndex = -1, QList< QSharedPointer<QObject> > oldItems = QList< QSharedPointer<QObject> >());

    ListChangedSignalArgs::Action action() const;
    int index() const;
    int oldIndex() const;
    QList< QSharedPointer<QObject> > items() const;
    QList< QSharedPointer<QObject> > oldItems() const;

private:
    Action m_action;
    int m_index;
    int m_oldIndex;
    QList< QSharedPointer<QObject> > m_items;
    QList< QSharedPointer<QObject> > m_oldItems;
};

} // ComponentModel

Q_DECLARE_METATYPE(ComponentModel::ListChangedSignalArgs)
Q_DECLARE_METATYPE(ComponentModel::ListChangedSignalArgs::Action)

#endif // COMPONENTMODEL_LISTCHANGEDSIGNALARGS_H

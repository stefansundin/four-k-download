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


#include "componentmodel/listchangedsignalargs.h"

using namespace ComponentModel;


ListChangedSignalArgs::ListChangedSignalArgs(QObject* sender, ListChangedSignalArgs::Action action,
                      int index, QList< QSharedPointer<QObject> > items,
                      int oldIndex, QList< QSharedPointer<QObject> > oldItems) :
    SignalArgs(sender),
    m_action(action),
    m_index(index),
    m_oldIndex(oldIndex),
    m_items(items),
    m_oldItems(oldItems)
{
}


ListChangedSignalArgs::Action ListChangedSignalArgs::action() const
{
    return m_action;
}


int ListChangedSignalArgs::index() const
{
    return m_index;
}


int ListChangedSignalArgs::oldIndex() const
{
    return m_oldIndex;
}


QList< QSharedPointer<QObject> > ListChangedSignalArgs::items() const
{
    return m_items;
}


QList< QSharedPointer<QObject> > ListChangedSignalArgs::oldItems() const
{
    return m_oldItems;
}

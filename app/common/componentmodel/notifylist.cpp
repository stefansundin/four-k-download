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


#include "componentmodel/notifylist.h"

using namespace ComponentModel;


NotifyList::NotifyList(QObject* parent) :
        QObject(parent)
{
}


void NotifyList::append(QSharedPointer<QObject> object)
{
    checkObject(object);
    m_list.append(object);

    QList< QSharedPointer<QObject> > items;
    items.append(object);

    emit listChanged(ListChangedSignalArgs(this, ListChangedSignalArgs::Insert, m_list.count() - 1, items));
}


void NotifyList::insert(int index, QSharedPointer<QObject> object)
{
    checkObject(object);
    m_list.insert(index, object);

    QList< QSharedPointer<QObject> > items;
    items.append(object);

    emit listChanged(ListChangedSignalArgs(this, ListChangedSignalArgs::Insert, index, items));
}


void NotifyList::replace(int index, QSharedPointer<QObject> object)
{
    QList< QSharedPointer<QObject> > oldItems;
    oldItems.append(m_list[index]);

    m_list.replace(index, object);

    QList< QSharedPointer<QObject> > items;
    items.append(object);

    emit listChanged(ListChangedSignalArgs(this, ListChangedSignalArgs::Replace, index, items, index, oldItems));
}


void NotifyList::remove(int index)
{
    QList< QSharedPointer<QObject> > oldItems;
    oldItems.append(m_list[index]);

    m_list.removeAt(index);

    emit listChanged(ListChangedSignalArgs(this, ListChangedSignalArgs::Remove, -1, QList< QSharedPointer<QObject> >(), index, oldItems));
}


void NotifyList::move(int fromIndex, int toIndex)
{
    QList< QSharedPointer<QObject> > oldItems;
    oldItems.append(m_list[fromIndex]);

    m_list.move(fromIndex, toIndex);

    emit listChanged(ListChangedSignalArgs(this, ListChangedSignalArgs::Move, toIndex, oldItems, fromIndex, oldItems));
}


void NotifyList::swap(int i, int j)
{
    QList< QSharedPointer<QObject> > oldItems;
    oldItems.append(m_list[i]);

    QList< QSharedPointer<QObject> > items;
    items.append(m_list[j]);

    m_list.swap(i, j);

    emit listChanged(ListChangedSignalArgs(this, ListChangedSignalArgs::Swap, j, oldItems, i, items));
}


void NotifyList::clear()
{
    if (m_list.isEmpty())
        return;

    QList< QSharedPointer<QObject> > oldItems;
    oldItems.append(m_list);

    m_list.clear();

    emit listChanged(ListChangedSignalArgs(this, ListChangedSignalArgs::Clear, -1, QList< QSharedPointer<QObject> >(), -1, oldItems));
}


int NotifyList::indexOf(const QObject* object, int fromIndex) const
{
    int count = m_list.count();

    for (int i = fromIndex; i < count; ++i)
    {
        if (m_list[i].data() == object)
            return i;
    }

    return -1;
}


int NotifyList::indexOf(QSharedPointer<QObject> object, int fromIndex) const
{
    return indexOf(object.data(), fromIndex);
}


QSharedPointer<QObject> NotifyList::at(int index)
{
    return m_list.at(index);
}


QSharedPointer<QObject> NotifyList::operator[](int index)
{
    return m_list.operator [](index);
}


int NotifyList::count() const
{
    return m_list.count();
}


bool NotifyList::isEmpty() const
{
    return m_list.isEmpty();
}


void NotifyList::checkObject(const QSharedPointer<QObject>& object)
{
    Q_UNUSED(object)
}

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


#include "componentmodel/notifyselection.h"

using namespace ComponentModel;


NotifySelection::NotifySelection(NotifyList* list, QObject* parent) :
    QObject(parent),
    m_currentIndex(-1)
{
    setList(list);
}


NotifyList* NotifySelection::list() const
{
    return m_list.data();
}


void NotifySelection::setList(NotifyList* value)
{
    if (m_list.data() != value)
    {
        if (m_list)
            disconnect(m_list);

        m_list = value;

        if (m_list)
            connect(m_list);

        setCurrentIndex(-1);
    }
}


int NotifySelection::currentIndex() const
{
    if (m_list.isNull())
        return -1;
    return m_currentIndex;
}


void NotifySelection::setCurrentIndex(int value)
{
    if (m_list.isNull())
        return;

    if (value >= m_list.data()->count())
        return;

    if (value < -1)
        value = -1;

    int previousIndex = m_currentIndex;
    m_currentIndex = value;
    emit selectionChanged(SelectionChangedSignalArgs(this, m_currentIndex, previousIndex));
}


bool NotifySelection::isValid() const
{
    return (m_currentIndex >= 0);
}


void NotifySelection::listChanged(const ListChangedSignalArgs& args)
{
    if (m_list.isNull())
        return;

    switch (args.action())
    {
    case ListChangedSignalArgs::Insert:
        setCurrentIndex(args.index());
        break;

    case ListChangedSignalArgs::Replace:
        setCurrentIndex(args.index());
        break;

    case ListChangedSignalArgs::Remove:
        if (m_list.data()->count() == 0)
            setCurrentIndex(-1);
        else if (args.oldIndex() >= m_list.data()->count())
            setCurrentIndex(m_list.data()->count() - 1);
        else
            setCurrentIndex(args.oldIndex());
        break;

    case ListChangedSignalArgs::Move:
    case ListChangedSignalArgs::Swap:
        setCurrentIndex(args.index());
        break;

    case ListChangedSignalArgs::Clear:
        setCurrentIndex(-1);
        break;

    default:
        break;
    }
}


void NotifySelection::connect(const QWeakPointer<NotifyList>& list)
{
    QObject::connect(list.data(), SIGNAL(listChanged(ComponentModel::ListChangedSignalArgs)),
                     this, SLOT(listChanged(ComponentModel::ListChangedSignalArgs)));
}


void NotifySelection::disconnect(const QWeakPointer<NotifyList>& list)
{
    QObject::disconnect(list.data(), SIGNAL(listChanged(ComponentModel::ListChangedSignalArgs)));
}

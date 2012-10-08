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


#include "qtsplitterbase.h"

QtSplitterBase::QtSplitterBase(QDeclarativeItem *parent)
    : QDeclarativeItem(parent)
{
}

QtSplitterAttached *QtSplitterBase::qmlAttachedProperties(QObject *object)
{
    return new QtSplitterAttached(object);
}

void QtSplitterAttached::setExpanding(bool expanding)
{
    m_expanding = expanding;
    emit expandingChanged(expanding);
}

void QtSplitterAttached::setMaximumWidth(qreal width)
{
    m_maximumWidth = width;
    emit maximumWidthChanged(width);
}

void QtSplitterAttached::setMinimumWidth(qreal width)
{
    m_minimumWidth = width;
    emit minimumWidthChanged(width);
}

void QtSplitterAttached::setMaximumHeight(qreal height)
{
    m_maximumHeight = height;
    emit maximumHeightChanged(height);
}

void QtSplitterAttached::setMinimumHeight(qreal height)
{
    m_minimumHeight = height;
    emit minimumHeightChanged(height);
}

QtSplitterAttached::QtSplitterAttached(QObject *object)
    : QObject(object),
      m_minimumWidth(-1),
      m_maximumWidth(-1),
      m_minimumHeight(-1),
      m_maximumHeight(-1),
      m_percentageSize(-1),
      m_itemIndex(-1),
      m_expanding(false)
{
}

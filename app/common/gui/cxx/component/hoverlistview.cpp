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


#include "gui/cxx/component/hoverlistview.h"
#include <QMouseEvent>


HoverListView::HoverListView(QWidget *parent) :
    QListView(parent)
{
}


void HoverListView::setHoverIndex(const QModelIndex& index)
{
    if (m_hoverIndex != index)
    {
        m_hoverIndex = index;
        emit hovered(m_hoverIndex);
    }
}


void HoverListView::mouseMoveEvent(QMouseEvent* event)
{
    QPoint pos = viewport()->mapFromGlobal(event->globalPos());
    setHoverIndex(indexAt(pos));

    QListView::mouseMoveEvent(event);
}


void HoverListView::leaveEvent(QEvent* event)
{
    Q_UNUSED(event)
    setHoverIndex(QModelIndex());

    QListView::leaveEvent(event);
}

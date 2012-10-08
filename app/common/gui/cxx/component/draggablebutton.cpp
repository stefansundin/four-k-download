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


#include "gui/cxx/component/draggablebutton.h"
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include <QApplication>


DraggableButton::DraggableButton(QWidget* parent) :
    ExtendedButton(parent)
{
    setAcceptDrops(true);
}


void DraggableButton::mousePressEvent(QMouseEvent* event)
{
    QPushButton::mousePressEvent(event);

    if (event->button() == Qt::LeftButton)
        m_dragStartPosition = event->pos();
}


void DraggableButton::mouseMoveEvent(QMouseEvent* event)
{
    if (!(event->buttons() & Qt::LeftButton))
        return;
    if ((event->pos() - m_dragStartPosition).manhattanLength() < QApplication::startDragDistance())
        return;

    QDrag* drag = new QDrag(this);
    QMimeData* mimeData = new QMimeData;
    mimeData->setData("application/draggablebutton", QByteArray());
    drag->setMimeData(mimeData);

    drag->exec(Qt::MoveAction);

    QMouseEvent r(QEvent::MouseButtonRelease, QPoint(-1, -1), Qt::LeftButton, Qt::MouseButtons(), Qt::KeyboardModifiers());
    QApplication::sendEvent(this, &r);
    QMouseEvent l(QEvent::Leave, QPoint(-1, -1), Qt::NoButton, Qt::MouseButtons(), Qt::KeyboardModifiers());
    QApplication::sendEvent(this, &l);
}


void DraggableButton::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->source() != this && event->mimeData()->hasFormat("application/draggablebutton"))
        event->acceptProposedAction();
}


void DraggableButton::dragLeaveEvent(QDragLeaveEvent* event)
{
}


void DraggableButton::dropEvent(QDropEvent* event)
{
    emit buttonDropped((DraggableButton*)event->source(), this);

    QMouseEvent l(QEvent::Leave, QPoint(-1, -1), Qt::NoButton, Qt::MouseButtons(), Qt::KeyboardModifiers());
    QApplication::sendEvent(this, &l);
}

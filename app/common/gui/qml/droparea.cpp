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


#include "gui/qml/droparea.h"
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>

using namespace Gui;


DropArea::DropArea(QDeclarativeItem *parent)
        : QDeclarativeItem(parent)
{
    setAcceptDrops(true);
}


void DropArea::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->hasUrls())
    {
        QUrl url;
        QList<QUrl> urls = event->mimeData()->urls();

        foreach (url, urls)
        {
            if (url.isLocalFile())
            {
                event->accept();
                setCursor(Qt::DragMoveCursor);
                return;
            }
        }
    }

    event->ignore();
}


void DropArea::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    unsetCursor();
}


void DropArea::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    unsetCursor();

    QUrl url;
    QList<QUrl> urls = event->mimeData()->urls();

    foreach (url, urls)
    {
        if (url.isLocalFile())
            emit fileDropped(url.toLocalFile());
    }
}

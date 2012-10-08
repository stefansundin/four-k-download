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


#ifndef GUI_QML_DROPAREA_H
#define GUI_QML_DROPAREA_H

#include <QDeclarativeItem>
#include "gui/qml/metatypes.h"

namespace Gui
{

class DropArea : public QDeclarativeItem
{
    Q_OBJECT

public:
    explicit DropArea(QDeclarativeItem *parent=0);

signals:
    void fileDropped(QString path);

protected:
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event);
    void dropEvent(QGraphicsSceneDragDropEvent *event);
};

} // Gui

#endif // GUI_QML_DROPAREA_H

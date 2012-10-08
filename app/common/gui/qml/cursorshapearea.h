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


#ifndef GUI_QML_CURSORSHAPEAREA_H
#define GUI_QML_CURSORSHAPEAREA_H

#include <QDeclarativeItem>
#include "gui/qml/metatypes.h"

namespace Gui
{

class CursorShapeArea : public QDeclarativeItem
{
    Q_OBJECT 
    Q_PROPERTY(Qt::CursorShape cursorShape READ cursorShape WRITE setCursorShape NOTIFY cursorShapeChanged)
 
public: 
    explicit CursorShapeArea(QDeclarativeItem *parent = 0);
 
    Qt::CursorShape cursorShape() const;
    void setCursorShape(Qt::CursorShape cursorShape);
 
signals:
    void cursorShapeChanged();

private:
    int m_currentShape; 
};

} // Gui

#endif // GUI_QML_CURSORSHAPEAREA_H

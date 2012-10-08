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


#include "gui/qml/cursorshapearea.h"

using namespace Gui;


CursorShapeArea::CursorShapeArea(QDeclarativeItem *parent) :
    QDeclarativeItem(parent),
    m_currentShape(-1)
{
}


Qt::CursorShape CursorShapeArea::cursorShape() const
{
    return cursor().shape();
}


void CursorShapeArea::setCursorShape(Qt::CursorShape cursorShape)
{
    if (m_currentShape == (int)cursorShape)
        return;
 
    m_currentShape = cursorShape;
    setCursor(cursorShape);
    
    emit cursorShapeChanged();
}
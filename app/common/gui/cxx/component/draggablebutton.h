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


#ifndef GUI_CXX_COMPONENT_DRAGGABLEBUTTON_H
#define GUI_CXX_COMPONENT_DRAGGABLEBUTTON_H

#include "extendedbutton.h"

class DraggableButton : public ExtendedButton
{
    Q_OBJECT

public:
    explicit DraggableButton(QWidget* parent = 0);

signals:
    void buttonDropped(DraggableButton* dragButton, DraggableButton* dropButton);

protected:
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void dragEnterEvent(QDragEnterEvent* event);
    virtual void dragLeaveEvent(QDragLeaveEvent* event);
    virtual void dropEvent(QDropEvent* event);

private:
    QPoint m_dragStartPosition;
};

#endif // GUI_CXX_COMPONENT_DRAGGABLEBUTTON_H

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


#ifndef GUI_CXX_COMPONENT_HOVERLISTVIEW_H
#define GUI_CXX_COMPONENT_HOVERLISTVIEW_H

#include <QListView>

class HoverListView : public QListView
{
    Q_OBJECT

public:
    explicit HoverListView(QWidget* parent = 0);

protected:
    void setHoverIndex(const QModelIndex& index);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void leaveEvent(QEvent* event);

signals:
    void hovered(const QModelIndex& index);

private:
    QModelIndex m_hoverIndex;
};

#endif // GUI_CXX_COMPONENT_HOVERLISTVIEW_H

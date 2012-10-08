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


#include "gui/cxx/component/volumeslider.h"
#include <QPainter>
#include <QStyleOptionSlider>


VolumeSlider::VolumeSlider(QWidget* parent) :
    QSlider(parent)
{
}


void VolumeSlider::activate()
{
    grabMouse();
    setFocus(Qt::TabFocusReason);
}


void VolumeSlider::deactivate()
{
    releaseMouse();
    emit leaved();
}


void VolumeSlider::mousePressEvent(QMouseEvent* event)
{
    QRect rect = QRect(0, 0, width(), height());
    if (!rect.contains(mapFromGlobal(event->globalPos())))
    {
        deactivate();
        return;
    }

    QSlider::mousePressEvent(event);
}



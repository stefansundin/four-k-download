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


#ifndef GUI_CXX_COMPONENT_VOLUMESLIDER_H
#define GUI_CXX_COMPONENT_VOLUMESLIDER_H

#include <QSlider>
#include <QPaintEvent>
#include <QStyle>

class VolumeSlider : public QSlider
{
    Q_OBJECT

public:
    explicit VolumeSlider(QWidget* parent = 0);

    void activate();
    void deactivate();

signals:
    void leaved();

protected:
    virtual void mousePressEvent(QMouseEvent* event);
};

#endif // GUI_CXX_COMPONENT_VOLUMESLIDER_H

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


#ifndef GUI_CXX_COMPONENT_EXTENDEDBUTTON_H
#define GUI_CXX_COMPONENT_EXTENDEDBUTTON_H

#include <QPushButton>

class ExtendedButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(Qt::Orientation contentOrientation READ contentOrientation WRITE setContentOrientation)
    Q_PROPERTY(int contentInterval READ contentInterval WRITE setContentInterval)
    Q_PROPERTY(int horizontalPadding READ horizontalPadding WRITE setHorizontalPadding)
    Q_PROPERTY(int verticalPadding READ verticalPadding WRITE setVerticalPadding)
    Q_PROPERTY(int normalOpacity READ normalOpacity WRITE setNormalOpacity)
    Q_PROPERTY(int hoveredOpacity READ hoveredOpacity WRITE setHoveredOpacity)
    Q_PROPERTY(int pressedOpacity READ pressedOpacity WRITE setPressedOpacity)

public:
    explicit ExtendedButton(QWidget* parent = 0);

    Qt::Orientation contentOrientation() const;
    void setContentOrientation(Qt::Orientation value);
    int contentInterval() const;
    void setContentInterval(int value);
    int horizontalPadding() const;
    void setHorizontalPadding(int value);
    int verticalPadding() const;
    void setVerticalPadding(int value);
    int normalOpacity() const;
    void setNormalOpacity(int value);
    int hoveredOpacity() const;
    void setHoveredOpacity(int value);
    int pressedOpacity() const;
    void setPressedOpacity(int value);

    virtual QSize sizeHint() const;

protected:
    virtual void paintEvent(QPaintEvent *);

private:
    Qt::Orientation m_contentOrientation;
    int m_contentInterval;
    int m_horizontalPadding;
    int m_verticalPadding;
    int m_normalOpacity;
    int m_hoveredOpacity;
    int m_pressedOpacity;
};

#endif // GUI_CXX_COMPONENT_EXTENDEDBUTTON_H

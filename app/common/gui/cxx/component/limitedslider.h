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


#ifndef GUI_CXX_COMPONENT_LIMITEDSLIDER_H
#define GUI_CXX_COMPONENT_LIMITEDSLIDER_H

#include <QSlider>
#include <QPaintEvent>
#include <QStyle>

class LimitedSlider : public QSlider
{
    Q_OBJECT
    Q_PROPERTY(int leftLimit READ leftLimit WRITE setLeftLimit NOTIFY leftLimitChanged USER true)
    Q_PROPERTY(int rightLimit READ rightLimit WRITE setRightLimit NOTIFY rightLimitChanged USER true)

public:
    explicit LimitedSlider(QWidget *parent = 0);

    int leftLimit() const;
    void setLeftLimit(int value);
    int rightLimit() const;
    void setRightLimit(int value);

signals:
    void leftLimitChanged(int value);
    void rightLimitChanged(int value);

protected:
    enum LimiterType { None, LimiterLeft, LimiterRight };

    void paintEvent(QPaintEvent* ev);
    void mousePressEvent(QMouseEvent* ev);
    void mouseMoveEvent(QMouseEvent* ev);
    void mouseReleaseEvent(QMouseEvent* ev);
    bool event(QEvent* event);

    void updateHoverControl(const QPoint& pos);
    int xToValue(int x, const QStyleOptionSlider& opt) const;
    int valueToX(int value, const QStyleOptionSlider& opt) const;
    QRect limiterRect(LimitedSlider::LimiterType type, const QStyleOptionSlider& opt) const;

private:
    QStyle::SubControl m_pressedControl;
    QStyle::SubControl m_hoverControl;
    LimitedSlider::LimiterType m_pressedLimiter;
    LimitedSlider::LimiterType m_hoverLimiter;
    int m_leftLimit;
    int m_rightLimit;
};

#endif // GUI_CXX_COMPONENT_LIMITEDSLIDER_H

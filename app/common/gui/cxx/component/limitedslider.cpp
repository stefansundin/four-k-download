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


#include "gui/cxx/component/limitedslider.h"
#include <QPainter>
#include <QStyleOptionSlider>


LimitedSlider::LimitedSlider(QWidget *parent) :
    QSlider(parent),
    m_pressedControl(QStyle::SC_None),
    m_hoverControl(QStyle::SC_None),
    m_pressedLimiter(LimitedSlider::None),
    m_hoverLimiter(LimitedSlider::None),
    m_leftLimit(0),
    m_rightLimit(99)
{
    setOrientation(Qt::Horizontal);
}


int LimitedSlider::leftLimit() const
{
    return m_leftLimit;
}


void LimitedSlider::setLeftLimit(int value)
{
    if (m_leftLimit != value)
    {
        if (value < minimum())
            value = minimum();
        if (value > m_rightLimit)
            value = m_rightLimit;

        m_leftLimit = value;

        emit leftLimitChanged(m_leftLimit);
        update();
    }
}


int LimitedSlider::rightLimit() const
{
    return m_rightLimit;
}


void LimitedSlider::setRightLimit(int value)
{
    if (m_rightLimit != value)
    {
        if (value > maximum())
            value = maximum();
        if (value < m_leftLimit)
            value = m_leftLimit;

        m_rightLimit = value;

        emit rightLimitChanged(m_rightLimit);
        update();
    }
}


void LimitedSlider::paintEvent(QPaintEvent* ev)
{
    QPainter p(this);
    QStyleOptionSlider opt;
    initStyleOption(&opt);


    opt.subControls = QStyle::SC_SliderGroove;
    if (m_pressedControl)
    {
        opt.activeSubControls = m_pressedControl;
        opt.state |= QStyle::State_Sunken;
    }
    else
    {
        opt.activeSubControls = m_hoverControl;
    }

    style()->drawComplexControl(QStyle::CC_Slider, &opt, &p, this);


    p.save();

    QColor color = opt.palette.background().color().darker();
    color.setAlpha(isEnabled() && (m_hoverLimiter == LimitedSlider::LimiterLeft || m_pressedLimiter == LimitedSlider::LimiterLeft) ?
                   255 : 100);
    p.setBrush(QBrush(color, Qt::SolidPattern));
    p.setPen(color);

    QRect rect = limiterRect(LimitedSlider::LimiterLeft, opt);
    QPolygon polygon;
    polygon.append(QPoint(rect.left(), rect.top()));
    polygon.append(QPoint(rect.left(), rect.bottom()));
    polygon.append(QPoint(rect.right(), rect.bottom()));
    p.drawPolygon(polygon);

    int left = rect.left();

    color.setAlpha(isEnabled() && (m_hoverLimiter == LimitedSlider::LimiterRight || m_pressedLimiter == LimitedSlider::LimiterRight) ?
                   255 : 100);
    p.setBrush(QBrush(color, Qt::SolidPattern));
    p.setPen(color);

    rect = limiterRect(LimitedSlider::LimiterRight, opt);
    polygon.clear();
    polygon.append(QPoint(rect.right(), rect.top()));
    polygon.append(QPoint(rect.right(), rect.bottom()));
    polygon.append(QPoint(rect.left(), rect.bottom()));
    p.drawPolygon(polygon);

    int right = rect.right();

    color = opt.palette.background().color().darker();
    color.setAlpha(50);
    p.setBrush(QBrush(color, Qt::SolidPattern));
    p.setPen(color);

    rect = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, this);
    rect.setTop(rect.center().y() - 1);
    rect.setBottom(rect.top() + 1);
    rect.setLeft(left < rect.left() + 1 ? rect.left() + 1 : left);
    rect.setRight(right > rect.right() - 2 ? rect.right() - 2 : right);
    p.drawRect(rect);

    p.restore();


    opt.subControls = QStyle::SC_SliderHandle;
    style()->drawComplexControl(QStyle::CC_Slider, &opt, &p, this);


    ev->accept();
}


void LimitedSlider::mousePressEvent(QMouseEvent* ev)
{
    m_pressedLimiter = m_hoverLimiter;
    if (m_pressedLimiter != LimitedSlider::None)
    {
        ev->accept();
        return;
    }

    QSlider::mousePressEvent(ev);

    if (!ev->isAccepted())
        return;

    if ((ev->button() & style()->styleHint(QStyle::SH_Slider_AbsoluteSetButtons)) == ev->button())
    {
        m_pressedControl = QStyle::SC_SliderHandle;
    }
    else if ((ev->button() & style()->styleHint(QStyle::SH_Slider_PageSetButtons)) == ev->button())
    {
        QStyleOptionSlider opt;
        initStyleOption(&opt);
        m_pressedControl = style()->hitTestComplexControl(QStyle::CC_Slider, &opt, ev->pos(), this);

        const QRect sliderRect = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
        setValue(xToValue(ev->pos().x(), opt));
    }
}


void LimitedSlider::mouseMoveEvent(QMouseEvent* ev)
{
    QSlider::mouseMoveEvent(ev);

    QStyleOptionSlider opt;
    initStyleOption(&opt);

    if (m_pressedLimiter == LimitedSlider::LimiterLeft)
        setLeftLimit(xToValue(ev->pos().x() - 2, opt));
    else if (m_pressedLimiter == LimitedSlider::LimiterRight)
        setRightLimit(xToValue(ev->pos().x() + 2, opt));
}


void LimitedSlider::mouseReleaseEvent(QMouseEvent* ev)
{
    QSlider::mouseReleaseEvent(ev);

    m_pressedControl = QStyle::SC_None;
    m_pressedLimiter = LimitedSlider::None;
}


bool LimitedSlider::event(QEvent* event)
{
    switch(event->type())
    {
    case QEvent::HoverEnter:
    case QEvent::HoverLeave:
    case QEvent::HoverMove:
        if (const QHoverEvent *he = static_cast<const QHoverEvent *>(event))
            updateHoverControl(he->pos());
        break;

    default:
        break;
    }

    return QSlider::event(event);
}


void LimitedSlider::updateHoverControl(const QPoint &pos)
{
    if (m_pressedControl != QStyle::SC_None || m_pressedLimiter != LimitedSlider::None)
        return;


    QStyleOptionSlider opt;
    initStyleOption(&opt);
    opt.subControls = QStyle::SC_All;

    QRect handleRect = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
    QRect grooveRect = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, this);
    QRect tickmarksRect = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderTickmarks, this);
    QRect leftRect = limiterRect(LimitedSlider::LimiterLeft, opt);
    QRect rightRect = limiterRect(LimitedSlider::LimiterRight, opt);


    if (handleRect.contains(pos))
        m_hoverControl = QStyle::SC_SliderHandle;
    else if (grooveRect.contains(pos))
        m_hoverControl = QStyle::SC_SliderGroove;
    else if (tickmarksRect.contains(pos))
        m_hoverControl = QStyle::SC_SliderTickmarks;
    else
        m_hoverControl = QStyle::SC_None;


    if (m_hoverControl != QStyle::SC_SliderHandle && leftRect.contains(pos))
    {
        m_hoverLimiter = LimitedSlider::LimiterLeft;
        setCursor(QCursor(Qt::SizeHorCursor));
    }
    else if (m_hoverControl != QStyle::SC_SliderHandle && rightRect.contains(pos))
    {
        m_hoverLimiter = LimitedSlider::LimiterRight;
        setCursor(QCursor(Qt::SizeHorCursor));
    }
    else
    {
        m_hoverLimiter = LimitedSlider::None;
        setCursor(QCursor(Qt::ArrowCursor));
    }

    update();
}


int LimitedSlider::xToValue(int x, const QStyleOptionSlider& opt) const
{
    QRect handleRect = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
    QRect grooveRect = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, this);
    return minimum() + (maximum() - minimum()) * (x - (grooveRect.left() + handleRect.width() / 2)) / (grooveRect.width() - handleRect.width());
}


int LimitedSlider::valueToX(int value, const QStyleOptionSlider& opt) const
{
    QRect handleRect = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
    QRect grooveRect = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, this);
    return (grooveRect.left() + handleRect.width() / 2) + (grooveRect.width() - handleRect.width()) * (value - minimum()) / (maximum() - minimum());
}


QRect LimitedSlider::limiterRect(LimitedSlider::LimiterType type, const QStyleOptionSlider& opt) const
{
    QRect result;
    QRect grooveRect = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, this);

    result.setTop((grooveRect.top() + grooveRect.bottom()) / 2 + 2);
    result.setBottom(grooveRect.bottom() - 1);

    switch (type)
    {
    case LimitedSlider::LimiterLeft:
        if (m_leftLimit == minimum())
            result.setLeft(grooveRect.left());
        else
            result.setLeft(valueToX(m_leftLimit, opt));
        result.setRight(result.left() + result.height() - 1);
        break;

    case LimitedSlider::LimiterRight:
        if (m_rightLimit == maximum())
            result.setRight(grooveRect.right() - 1);
        else
            result.setRight(valueToX(m_rightLimit, opt) - 1);
        result.setLeft(result.right() - result.height() + 1);
        break;

    default:
        break;
    }

    return result;
}

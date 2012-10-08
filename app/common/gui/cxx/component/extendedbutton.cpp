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


#include "gui/cxx/component/extendedbutton.h"
#include <QStylePainter>
#include <QStyleOption>
#include <QDialogButtonBox>
#include <QApplication>


ExtendedButton::ExtendedButton(QWidget *parent) :
    QPushButton(parent),
    m_contentOrientation(Qt::Horizontal),
    m_contentInterval(0),
    m_horizontalPadding(-1),
    m_verticalPadding(-1),
    m_normalOpacity(50),
    m_hoveredOpacity(100),
    m_pressedOpacity(100)
{
}


Qt::Orientation ExtendedButton::contentOrientation() const
{
    return m_contentOrientation;
}


void ExtendedButton::setContentOrientation(Qt::Orientation value)
{
    if (m_contentOrientation != value)
    {
        m_contentOrientation = value;
        update();
    }
}


int ExtendedButton::contentInterval() const
{
    return m_contentInterval;
}


void ExtendedButton::setContentInterval(int value)
{
    if (m_contentInterval != value)
    {
        m_contentInterval = value;
        update();
    }
}


int ExtendedButton::horizontalPadding() const
{
    return m_horizontalPadding;
}


void ExtendedButton::setHorizontalPadding(int value)
{
    if (m_horizontalPadding != value)
    {
        m_horizontalPadding = value;
        update();
    }
}


int ExtendedButton::verticalPadding() const
{
    return m_verticalPadding;
}


void ExtendedButton::setVerticalPadding(int value)
{
    if (m_verticalPadding != value)
    {
        m_verticalPadding = value;
        update();
    }
}


int ExtendedButton::normalOpacity() const
{
    return m_normalOpacity;
}


void ExtendedButton::setNormalOpacity(int value)
{
    if (m_normalOpacity != value)
    {
        m_normalOpacity = value;
        update();
    }
}


int ExtendedButton::hoveredOpacity() const
{
    return m_hoveredOpacity;
}


void ExtendedButton::setHoveredOpacity(int value)
{
    if (m_hoveredOpacity != value)
    {
        m_hoveredOpacity = value;
        update();
    }
}


int ExtendedButton::pressedOpacity() const
{
    return m_pressedOpacity;
}


void ExtendedButton::setPressedOpacity(int value)
{
    if (m_pressedOpacity != value)
    {
        m_pressedOpacity = value;
        update();
    }
}


QSize ExtendedButton::sizeHint() const
{
    int w = 0, h = 0;

    QStyleOptionButton opt;
    initStyleOption(&opt);

    // calculate contents size...
    bool showButtonBoxIcons = qobject_cast<QDialogButtonBox*>(parentWidget())
                          && style()->styleHint(QStyle::SH_DialogButtonBox_ButtonsHaveIcons);

    if (!icon().isNull() || showButtonBoxIcons)
    {
        int ih = opt.iconSize.height();
        int iw = opt.iconSize.width() + 4;
        w += iw;
        h = qMax(h, ih);
    }

    QString s(text());
    bool empty = s.isEmpty();
    if (empty)
        s = QString::fromLatin1("XXXX");
    QFontMetrics fm = fontMetrics();
    QSize sz = fm.size(Qt::TextShowMnemonic, s);

    if (m_contentOrientation == Qt::Horizontal)
    {
        if(!empty || !w)
            w += sz.width();
        if(!empty || !h)
            h = qMax(h, sz.height());
    }
    else
    {
        if(!empty || !w)
            w = qMax(w, sz.width());
        if (!empty && !h)
            h += m_contentInterval;
        if(!empty || !h)
            h += sz.height();
    }

    opt.rect.setSize(QSize(w, h)); // PM_MenuButtonIndicator depends on the height

    if (menu())
        w += style()->pixelMetric(QStyle::PM_MenuButtonIndicator, &opt, this);

    if (m_horizontalPadding < 0 && m_verticalPadding < 0)
    {
        return (style()->sizeFromContents(QStyle::CT_PushButton, &opt, QSize(w, h), this).
                expandedTo(QApplication::globalStrut()));
    }
    else
    {
        return QSize(w + 2 * m_horizontalPadding, h + 2 * m_verticalPadding);
    }
}


void ExtendedButton::paintEvent(QPaintEvent* event)
{
    if (!isVisible())
        return;

    QStylePainter p(this);
    QStyleOptionButton option;
    initStyleOption(&option);

    if (option.features & QStyleOptionButton::Flat)
    {
        if (isChecked() || isDown())
            p.setOpacity(m_pressedOpacity / 100.0);
        else if (underMouse())
            p.setOpacity(m_hoveredOpacity / 100.0);
        else
            p.setOpacity(m_normalOpacity / 100.0);

        option.features = option.features & ~QStyleOptionButton::Flat;
    }

    p.drawControl(QStyle::CE_PushButtonBevel, option);

    if (p.opacity() != 1.0)
        p.setOpacity(1.0);

    if (m_contentOrientation == Qt::Horizontal)
    {
        p.drawControl(QStyle::CE_PushButtonLabel, option);
    }
    else
    {
        long x = option.rect.left() + (option.rect.width() - option.iconSize.width()) / 2;
        long y = option.rect.top() + (option.rect.height() - option.iconSize.height() -
                                      m_contentInterval - option.fontMetrics.height()) / 2;

        p.drawPixmap(x, y, option.iconSize.width(), option.iconSize.height(), option.icon.pixmap(option.iconSize));

        y += option.iconSize.height() + m_contentInterval;

        p.drawText(QRect(option.rect.left(), y, option.rect.width(), option.fontMetrics.height()), Qt::AlignHCenter | Qt::AlignTop, option.text);
    }
}

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


#include "gui/cxx/component/flowlayout.h"
#include <QtGui>


FlowLayout::FlowLayout(QWidget* parent, int margin, int hSpacing, int vSpacing) :
    QLayout(parent),
    m_hSpace(hSpacing),
    m_vSpace(vSpacing),
    m_contentsAlignment(Qt::AlignLeft)
{
    setContentsMargins(margin, margin, margin, margin);
}


FlowLayout::FlowLayout(int margin, int hSpacing, int vSpacing) :
    m_hSpace(hSpacing),
    m_vSpace(vSpacing),
    m_contentsAlignment(Qt::AlignLeft)
{
    setContentsMargins(margin, margin, margin, margin);
}


FlowLayout::~FlowLayout()
{
    QLayoutItem* item;
    while ((item = takeAt(0)))
        delete item;
}


void FlowLayout::addItem(QLayoutItem *item)
{
    itemList.append(item);
}


void FlowLayout::swapItems(int i, int j)
{
    itemList.swap(i, j);
}


int FlowLayout::horizontalSpacing() const
{
    if (m_hSpace >= 0)
        return m_hSpace;

    return smartSpacing(QStyle::PM_LayoutHorizontalSpacing);
}


int FlowLayout::verticalSpacing() const
{
    if (m_vSpace >= 0)
        return m_vSpace;

    return smartSpacing(QStyle::PM_LayoutVerticalSpacing);
}


int FlowLayout::count() const
{
    return itemList.size();
}


QLayoutItem* FlowLayout::itemAt(int index) const
{
    return itemList.value(index);
}


QLayoutItem* FlowLayout::takeAt(int index)
{
    if (index >= 0 && index < itemList.size())
        return itemList.takeAt(index);
    else
        return 0;
}


Qt::Orientations FlowLayout::expandingDirections() const
{
    return 0;
}


bool FlowLayout::hasHeightForWidth() const
{
    return true;
}


int FlowLayout::heightForWidth(int width) const
{
    int height = doLayout(QRect(0, 0, width, 0), true);
    return height;
}


void FlowLayout::setGeometry(const QRect &rect)
{
    QLayout::setGeometry(rect);
    doLayout(rect, false);
}


QSize FlowLayout::sizeHint() const
{
    return minimumSize();
}


QSize FlowLayout::minimumSize() const
{
    QSize size;
    QLayoutItem* item;
    foreach (item, itemList)
        size = size.expandedTo(item->minimumSize());

    size += QSize(2*margin(), 2*margin());
    return size;
}

int rowCount(QList<QRect> &rectList, int index)
{
    int top = rectList.at(index).top();
    int count = rectList.count();
    int i = index + 1;
    while (i < count)
    {
        if (rectList.at(i).top() != top)
            break;
        i += 1;
    }
    return i - index;
}

int FlowLayout::doLayout(const QRect &rect, bool testOnly) const
{
    int left, top, right, bottom;
    getContentsMargins(&left, &top, &right, &bottom);
    QRect effectiveRect = rect.adjusted(+left, +top, -right, -bottom);
    int x = effectiveRect.x();
    int y = effectiveRect.y();
    int lineWidth = 0;
    int lineHeight = 0;
    QList<QRect> rectList;
    QLayoutItem *item;

    foreach (item, itemList)
    {
        QWidget* wid = item->widget();
        int spaceX = horizontalSpacing();
        if (spaceX == -1)
            spaceX = wid->style()->layoutSpacing(
                    QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Horizontal);
        int spaceY = verticalSpacing();
        if (spaceY == -1)
            spaceY = wid->style()->layoutSpacing(
                    QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Vertical);
        int nextX = x + item->sizeHint().width() + spaceX;
        if (nextX - spaceX > effectiveRect.right() && lineHeight > 0)
        {
            x = effectiveRect.x();
            y = y + lineHeight + spaceY;
            nextX = x + item->sizeHint().width() + spaceX;
            lineHeight = 0;
        }

        if (!testOnly)
            rectList.append(QRect(QPoint(x, y), item->sizeHint()));

        x = nextX;
        lineWidth = qMax(lineWidth, x);
        lineHeight = qMax(lineHeight, item->sizeHint().height());
    }

    if (!testOnly)
    {
        if (m_contentsAlignment & Qt::AlignJustify)
        {
            // Stretch to all available width

            int index = 0;
            int count = rectList.count();
            while (index < count)
            {
                int length = rowCount(rectList, index);
                int right = rectList.at(index + length - 1).right();
                int offset = (effectiveRect.right() - right) / length;
                for (int i = index, j = 0; i < index + length; ++i, ++j)
                {
                    QRect rect = rectList.at(i);
                    rect.moveLeft(rect.left() + offset * j);
                    if (i != length - 1)
                        rect.setRight(rect.right() + offset);
                    else
                        rect.setRight(effectiveRect.right());
                    itemList.at(i)->setGeometry(rect);
                }
                index = index + length;
            }
        }
        else
        {
            // Horizontal alignment

            int offset = 0;
            if (m_contentsAlignment & Qt::AlignRight)
                offset = effectiveRect.right() - lineWidth;
            else if (m_contentsAlignment & Qt::AlignHCenter)
                offset = (effectiveRect.right() - lineWidth) / 2;

            int count = itemList.count();
            for (int i = 0; i < count; ++i)
            {
                QRect rect = rectList.at(i);
                rect.moveLeft(rect.left() + offset);
                itemList.at(i)->setGeometry(rect);
            }
        }
    }

    return y + lineHeight - rect.y() + bottom;
}


int FlowLayout::smartSpacing(QStyle::PixelMetric pm) const
{
    QObject *parent = this->parent();
    if (!parent)
    {
        return -1;
    }
    else if (parent->isWidgetType())
    {
        QWidget *pw = static_cast<QWidget *>(parent);
        return pw->style()->pixelMetric(pm, 0, pw);
    }
    else
    {
        return static_cast<QLayout *>(parent)->spacing();
    }
}


Qt::Alignment FlowLayout::contentsAlignment() const
{
    return m_contentsAlignment;
}


void FlowLayout::setContentsAlignment(Qt::Alignment value)
{
    if (m_contentsAlignment != value)
    {
        m_contentsAlignment = value;
        doLayout(geometry(), false);
    }
}

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


#include "qdesktopitem.h"

QDesktopItem::QDesktopItem(QObject* obj) : QObject(obj) {
    connect(&desktopWidget, SIGNAL(resized(int)), this, SIGNAL(screenGeometryChanged()));
    connect(&desktopWidget, SIGNAL(resized(int)), this, SIGNAL(availableGeometryChanged()));
    connect(&desktopWidget, SIGNAL(workAreaResized(int)), this, SIGNAL(availableGeometryChanged()));
    connect(&desktopWidget, SIGNAL(screenCountChanged(int)), this, SIGNAL(screenCountChanged()));
}

int QDesktopItem::screenCount() const
{
    return desktopWidget.screenCount();
}

QRect QDesktopItem::screenGeometry(int screen) const {
    return desktopWidget.screenGeometry(screen);
}

QRect QDesktopItem::availableGeometry(int screen) const {
    return desktopWidget.availableGeometry(screen);
}

int QDesktopItem::screenWidth() const
{
    return desktopWidget.screenGeometry().width();
}

int QDesktopItem::screenHeight() const
{
    return desktopWidget.screenGeometry().height();
}

int QDesktopItem::availableWidth() const
{
    return desktopWidget.availableGeometry().width();
}

int QDesktopItem::availableHeight() const
{
    return desktopWidget.availableGeometry().height();
}

QDesktopItem *QDesktopItem::qmlAttachedProperties(QObject *obj) {
    return new QDesktopItem(obj);
}

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


#ifndef COMPONENTMODEL_TRANSFORM_H
#define COMPONENTMODEL_TRANSFORM_H

#include <QObject>
#include <QString>
#include <QTime>
#include <QPixmap>

namespace ComponentModel
{

class Transform : public QObject
{
    Q_OBJECT

public:
    Q_INVOKABLE static QString timeToString(qint64 time, const QString& format = "hh:mm:ss.zzz");
    Q_INVOKABLE static QString storageSizeToString(qint64 size, const QString& format = "");
    Q_INVOKABLE static QPixmap base64ToPixmap(const QString& base64);
    Q_INVOKABLE static QString pixmapToBase64(const QPixmap& pixmap);
};

} // ComponentModel

#endif // COMPONENTMODEL_TRANSFORM_H

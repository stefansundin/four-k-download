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


#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <QString>
#include <QTime>
#include <QPixmap>

namespace ComponentModel
{

class Transform
{
public:
    static QString timeToString(qint64 time, const QString& format = "hh:mm:ss.zzz");
    static QString storageSizeToString(qint64 size, const QString& format = "");
    static QPixmap base64ToPixmap(const QString& base64);
    static QString pixmapToBase64(const QPixmap& pixmap);
};

} // ComponentModel

#endif // TRANSFORM_H

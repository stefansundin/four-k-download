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


#include "componentmodel/transform.h"
#include <QLocale>

using namespace ComponentModel;


QString Transform::timeToString(qint64 time, const QString& format)
{
    int h = time / 3600000; time -= h * 3600000;
    int m = time / 60000; time -= m * 60000;
    int s = time / 1000; time -= s * 1000;
    int ms = time;

    if (format == "remain")
    {
        if (h > 0)
        {
            if (m > 30)
                h += 1;

            return QObject::tr("%1 hour").arg(h);
        }

        if (m > 0)
        {
            if (s > 30 && m != 59)
                m += 1;

            return QObject::tr("%1 min").arg(m);
        }

        if (s / 10 > 0)
        {
            if (s % 10 > 2 && s / 10 != 5)
                s = s + 9;

            return QObject::tr("%1 sec").arg((s / 10) * 10);
        }

        return QObject::tr("%1 sec").arg(s);
    }

    return QTime(h, m, s, ms).toString(format);
}


QString Transform::storageSizeToString(qint64 size, const QString& format)
{
    if (size < 1024)
        return QString("%1 b").arg(size);

    QLocale locale;
    double reminder = size / 1024.0;

    if (reminder < 1024 || format.toUpper() == "K")
        return QString("%1 kB").arg(locale.toString(reminder, 'f', 1));

    reminder /= 1024;

    if (reminder < 1024 || format.toUpper() == "M")
    {
        QString str = locale.toString(reminder, 'f', 1);
        return QString("%1 MB").arg(str);
    }

    reminder /= 1024;

    if (reminder < 1024 || format.toUpper() == "G")
        return QString("%1 GB").arg(locale.toString(reminder, 'f', 1));

    reminder /= 1024;

    return QString("%1 TB").arg(locale.toString(reminder, 'f', 1));
}


QPixmap Transform::base64ToPixmap(const QString& base64)
{
    if (base64.isNull())
        return QPixmap();

    QByteArray array = QByteArray::fromBase64(base64.toAscii());
    if (array.isEmpty())
        return QPixmap();

    QDataStream stream(&array, QIODevice::ReadOnly);

    QPixmap pixmap;
    stream >> pixmap;
    return pixmap;
}


QString Transform::pixmapToBase64(const QPixmap& pixmap)
{
    if (pixmap.isNull())
        return QString();

    QByteArray array;
    QDataStream stream(&array, QIODevice::WriteOnly);
    stream << pixmap;

    if (array.isEmpty())
        return QString();

    return QString::fromAscii(array.toBase64());
}

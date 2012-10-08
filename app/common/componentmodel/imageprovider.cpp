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


#include "componentmodel/imageprovider.h"

using namespace ComponentModel;


ImageProvider::ImageProvider() :
    QDeclarativeImageProvider(QDeclarativeImageProvider::Pixmap)
{
}


void ImageProvider::updatePixmap(QString id, const QPixmap& pixmap)
{
    m_pixmaps.insert(id, pixmap);
}


QPixmap ImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    QPixmap result;

    if (m_pixmaps.contains(id))
        result = m_pixmaps.value(id, result);

    if (size)
        *size = result.size();

    if (!result.size().isEmpty() && !requestedSize.isEmpty())
        result = result.scaled(requestedSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    return result;
}

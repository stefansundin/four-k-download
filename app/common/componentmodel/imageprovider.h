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


#ifndef COMPONENTMODEL_IMAGEPROVIDER_H
#define COMPONENTMODEL_IMAGEPROVIDER_H

#include <QDeclarativeImageProvider>
#include <QMap>

namespace ComponentModel
{

class ImageProvider : public QDeclarativeImageProvider
{
public:
    ImageProvider();

    void updatePixmap(QString id, const QPixmap& pixmap);
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);

private:
    QMap<QString, QPixmap> m_pixmaps;

    Q_DISABLE_COPY(ImageProvider)
};

} // ComponentModel

#endif // COMPONENTMODEL_IMAGEPROVIDER_H

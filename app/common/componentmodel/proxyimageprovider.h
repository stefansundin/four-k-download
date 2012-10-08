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


#ifndef COMPONENTMODEL_PROXYIMAGEPROVIDER_H
#define COMPONENTMODEL_PROXYIMAGEPROVIDER_H

#include <QDeclarativeImageProvider>

namespace ComponentModel
{

class ProxyImageProvider : public QDeclarativeImageProvider
{
public:
    explicit ProxyImageProvider(QDeclarativeImageProvider* provider);

    QImage requestImage(const QString &id, QSize *size, const QSize& requestedSize);
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize& requestedSize);

private:
    QDeclarativeImageProvider* m_provider;
};

} // ComponentModel

#endif // COMPONENTMODEL_PROXYIMAGEPROVIDER_H

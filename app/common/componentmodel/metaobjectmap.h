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


#ifndef COMPONENTMODEL_METAOBJECTMAP_H
#define COMPONENTMODEL_METAOBJECTMAP_H

#include <QObject>
#include <QMap>

namespace ComponentModel
{

class MetaObjectMap : public QObject
{
    Q_OBJECT

public:
    explicit MetaObjectMap(QObject* parent = 0);

    void add(const QMetaObject& key, const QMetaObject& value);
    void remove(const QMetaObject& key);
    void reset();

    QObject* create(const QMetaObject& key, QGenericArgument arg1 = QGenericArgument(), QGenericArgument arg2 = QGenericArgument()) const;
    QObject* create(QObject* keyObject, QGenericArgument arg1 = QGenericArgument(), QGenericArgument arg2 = QGenericArgument()) const;

private:
    QMap<QMetaObject, QMetaObject> m_map;
};

} // ComponentModel

#endif // COMPONENTMODEL_METAOBJECTMAP_H

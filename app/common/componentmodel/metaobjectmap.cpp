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


#include "componentmodel/metaobjectmap.h"
#include <stdexcept>

using namespace ComponentModel;


MetaObjectMap::MetaObjectMap(QObject *parent) :
    QObject(parent)
{
}


void MetaObjectMap::add(const QMetaObject& key, const QMetaObject& value)
{
    if (m_map.contains(key))
        throw std::invalid_argument("Object already registered.");

    m_map.insert(key, value);
}


void MetaObjectMap::remove(const QMetaObject& key)
{
    m_map.remove(key);
}


void MetaObjectMap::reset()
{
    m_map.clear();
}


QObject* MetaObjectMap::create(const QMetaObject& key, QGenericArgument arg1, QGenericArgument arg2) const
{
    if (m_map.contains(key))
    {
        QMetaObject metaObject = m_map[key];
        return metaObject.newInstance(arg1, arg2);
    }

    return NULL;
}


QObject* MetaObjectMap::create(QObject* keyObject, QGenericArgument arg1, QGenericArgument arg2) const
{
    return create(*keyObject->metaObject(), arg1, arg2);
}


bool operator < (const QMetaObject & o1, const QMetaObject & o2)
{
    return o1.className() < o2.className();
}

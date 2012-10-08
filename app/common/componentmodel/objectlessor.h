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


#ifndef COMPONENTMODEL_OBJECTLESSOR_H
#define COMPONENTMODEL_OBJECTLESSOR_H

#include <QObject>
#include <QWeakPointer>
#include <componentmodel/objectlessee.h>

namespace ComponentModel
{

class ObjectLessor : public QObject
{
    Q_OBJECT

public:
    ObjectLessor(QObject* object, QObject* parent = 0);
    virtual ~ObjectLessor();

    QObject* leaseObject(IObjectLessee* lessee);
    void releaseObject(IObjectLessee* lessee);

private:
    QWeakPointer<QObject> m_object;
    IObjectLessee* m_lessee;

    Q_DISABLE_COPY(ObjectLessor)
};

} // ComponentModel

#endif // COMPONENTMODEL_OBJECTLESSOR_H

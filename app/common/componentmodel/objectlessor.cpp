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


#include "componentmodel/objectlessor.h"

using namespace ComponentModel;


ObjectLessor::ObjectLessor(QObject* object, QObject* parent) :
    QObject(parent),
    m_object(object),
    m_lessee(NULL)
{
}


ObjectLessor::~ObjectLessor()
{
    releaseObject(m_lessee);
}


QObject* ObjectLessor::leaseObject(IObjectLessee* lessee)
{
    releaseObject(m_lessee);

    if (m_lessee || m_object.isNull())
        return NULL;

    m_lessee = lessee;
    return m_object.data();
}


void ObjectLessor::releaseObject(IObjectLessee* lessee)
{
    if (m_lessee && m_lessee == lessee && m_lessee->releaseLeasableObject(this))
        m_lessee = NULL;
}

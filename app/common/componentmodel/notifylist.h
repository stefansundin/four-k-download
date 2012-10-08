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


#ifndef COMPONENTMODEL_NOTIFYLIST_H
#define COMPONENTMODEL_NOTIFYLIST_H

#include <QSharedPointer>
#include "componentmodel/listchangedsignalargs.h"

namespace ComponentModel
{

class NotifyList : public QObject
{
    Q_OBJECT

public:
    explicit NotifyList(QObject* parent = 0);

    void append(QSharedPointer<QObject> object);
    void insert(int index, QSharedPointer<QObject> object);
    void replace(int index, QSharedPointer<QObject> object);
    void remove(int index);
    void move(int fromIndex, int toIndex);
    void swap(int i, int j);
    void clear();

    int indexOf(const QObject* object, int fromIndex = 0) const;
    int indexOf(QSharedPointer<QObject> object, int fromIndex = 0) const;

    QSharedPointer<QObject> at(int index);
    QSharedPointer<QObject> operator[](int index);

    int count() const;
    bool isEmpty() const;

signals:
    void listChanged(const ComponentModel::ListChangedSignalArgs& args);

private:
    virtual void checkObject(const QSharedPointer<QObject>& object);

private:
    QList< QSharedPointer<QObject> > m_list;
};

} // ComponentModel

#endif // COMPONENTMODEL_NOTIFYLIST_H

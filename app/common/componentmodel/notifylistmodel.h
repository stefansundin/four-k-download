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


#ifndef COMPONENTMODEL_UNIVERSALLISTMODEL_H
#define COMPONENTMODEL_UNIVERSALLISTMODEL_H

#include <QObject>
#include <QAbstractItemModel>
#include <QWeakPointer>
#include <QStringList>
#include "componentmodel/notifylist.h"

namespace ComponentModel
{

class NotifyListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    NotifyListModel(NotifyList* list, QObject *parent = 0);

    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex& index, int role) const;
    virtual Qt::ItemFlags flags(const QModelIndex& index) const;

protected slots:
    void listChanged(const ComponentModel::ListChangedSignalArgs& args);

private:
    QWeakPointer<NotifyList> m_list;
};

} // ComponentModel

#endif // COMPONENTMODEL_UNIVERSALLISTMODEL_H

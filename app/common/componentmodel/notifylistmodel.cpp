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


#include "notifylistmodel.h"
#include <QMetaProperty>

using namespace ComponentModel;


NotifyListModel::NotifyListModel(NotifyList* list, QObject *parent) :
    QAbstractListModel(parent),
    m_list(list)
{
    QHash<int, QByteArray> roles;
    roles[Qt::UserRole + 1] = "object";
    setRoleNames(roles);

    QObject::connect(list, SIGNAL(listChanged(ComponentModel::ListChangedSignalArgs)),
                     this, SLOT(listChanged(ComponentModel::ListChangedSignalArgs)));
}


int NotifyListModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)

    if (m_list.isNull())
        return 0;

    return m_list.data()->count();
}


QVariant NotifyListModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || m_list.isNull() || index.row() >= m_list.data()->count())
        return QVariant();

    role = role - Qt::UserRole - 1; // UserRole is reference point

    if (role == 0)
        return QVariant::fromValue<QObject*>(m_list.data()->at(index.row()).data());

    return QVariant();
}


Qt::ItemFlags NotifyListModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return /*Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | */QAbstractItemModel::flags(index);
}


void NotifyListModel::listChanged(const ComponentModel::ListChangedSignalArgs& args)
{
    switch (args.action())
    {
    case ListChangedSignalArgs::Insert:
        beginInsertRows(QModelIndex(), args.index(), args.index() + args.items().count() - 1);
        endInsertRows();
        break;

    case ListChangedSignalArgs::Replace:
        beginRemoveRows(QModelIndex(), args.oldIndex(), args.oldIndex() + args.oldItems().count() - 1);
        endRemoveRows();
        beginInsertRows(QModelIndex(), args.index(), args.index() + args.items().count() - 1);
        endInsertRows();
        break;

    case ListChangedSignalArgs::Remove:
        beginRemoveRows(QModelIndex(), args.oldIndex(), args.oldIndex() + args.oldItems().count() - 1);
        endRemoveRows();
        break;

    case ListChangedSignalArgs::Move:
        beginRemoveRows(QModelIndex(), args.oldIndex(), args.oldIndex() + args.oldItems().count() - 1);
        endRemoveRows();
        beginInsertRows(QModelIndex(), args.index(), args.index() + args.items().count() - 1);
        endInsertRows();
        break;

    case ListChangedSignalArgs::Swap:
    case ListChangedSignalArgs::Clear:
        beginResetModel();
        endResetModel();
        break;

    default:
        break;
    }
}

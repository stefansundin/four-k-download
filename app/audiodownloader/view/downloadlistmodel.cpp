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


#include "view/downloadlistmodel.h"

using namespace View;
using namespace ViewModel;
using namespace ComponentModel;


DownloadListModel::DownloadListModel(QObject *parent) :
    QAbstractListModel(parent)
{
    qRegisterMetaType<Role>();
}


QObject* DownloadListModel::viewModel() const
{
    return m_viewModel.data();
}


void DownloadListModel::setViewModel(QObject* value)
{
    if (m_viewModel.data() != value)
    {
        beginResetModel();

        if (m_viewModel)
            disconnect(m_viewModel);

        m_viewModel = dynamic_cast<DownloadListViewModel*>(value);

        if (m_viewModel)
            connect(m_viewModel);

        endResetModel();
    }
}


int DownloadListModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)

    if (m_viewModel.isNull())
        return 0;
    return m_viewModel.data()->count();
}


QVariant DownloadListModel::data(const QModelIndex& index, int role) const
{
    if (m_viewModel.isNull() || !index.isValid() || index.row() >= m_viewModel.data()->count())
        return QVariant();

    switch (role)
    {
    case TitleRole:
        return m_viewModel.data()->at(index.row()).staticCast<DownloadItemViewModel>()->title();

    case DurationRole:
        return m_viewModel.data()->at(index.row()).staticCast<DownloadItemViewModel>()->duration();

    case SizeRole:
        return m_viewModel.data()->at(index.row()).staticCast<DownloadItemViewModel>()->size();

    case ProgressRole:
        return m_viewModel.data()->at(index.row()).staticCast<DownloadItemViewModel>()->progress();

    case RemainingTimeRole:
        return m_viewModel.data()->at(index.row()).staticCast<DownloadItemViewModel>()->remainingTime();

    case PlayingTimeRole:
        return m_viewModel.data()->at(index.row()).staticCast<DownloadItemViewModel>()->playingTime();

    case StateRole:
        return QVariant::fromValue(m_viewModel.data()->at(index.row()).staticCast<DownloadItemViewModel>()->state());

    default:
        return QVariant();
    }
}


Qt::ItemFlags DownloadListModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return /*Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | */QAbstractItemModel::flags(index);
}


Qt::DropActions DownloadListModel::supportedDropActions() const
{
    return Qt::MoveAction;
}


QStringList DownloadListModel::mimeTypes() const
{
    QStringList types;
    types << "application/download-item";
    return types;
}


QMimeData* DownloadListModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;
    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    foreach (QModelIndex index, indexes)
    {
        if (index.isValid())
            stream << index.row();
    }

    mimeData->setData("application/download-item", encodedData);
    return mimeData;
}


bool DownloadListModel::dropMimeData(const QMimeData *data, Qt::DropAction action,
                                  int row, int column, const QModelIndex &parent)
{
    Q_UNUSED(column)

    if (action == Qt::IgnoreAction)
        return true;

    if (!data->hasFormat("application/download-item") || m_viewModel.isNull())
        return false;

    int dstRow;
    if (row != -1)
        dstRow = row;
    else if (parent.isValid())
        dstRow = parent.row();
    else
        dstRow = rowCount(QModelIndex());

    QByteArray encodedData = data->data("application/download-item");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);

    int srcRow = 0;
    if (!stream.atEnd())
        stream >> srcRow;

    m_viewModel.data()->move(srcRow, dstRow);

    return true;
}


void DownloadListModel::listChanged(const ListChangedSignalArgs& args)
{
    if (m_viewModel.isNull())
        return;

    QList< QSharedPointer<QObject> > items = args.items();
    QList< QSharedPointer<QObject> > oldItems = args.oldItems();

    switch (args.action())
    {
    case ListChangedSignalArgs::Insert:
        connect(items);
        break;

    case ListChangedSignalArgs::Replace:
        disconnect(oldItems);
        connect(items);
        break;

    case ListChangedSignalArgs::Remove:
        disconnect(oldItems);
        break;

    case ListChangedSignalArgs::Clear:
        disconnect(oldItems);
        break;

    default:
        break;
    }

    beginResetModel();
    endResetModel();
}


void DownloadListModel::propertyChanged(const PropertyChangedSignalArgs& args)
{
    Q_UNUSED(args)

    int index = m_viewModel.data()->indexOf(args.sender());
    if (index != -1)
        emit dataChanged(this->index(index), this->index(index));
}


void DownloadListModel::connect(const QWeakPointer<DownloadListViewModel>& list)
{
    for (int i = list.data()->count() - 1; i >= 0; --i)
        connect(list.data()->at(i).staticCast<DownloadItemViewModel>());

    QObject::connect(list.data(), SIGNAL(listChanged(ComponentModel::ListChangedSignalArgs)),
                     this, SLOT(listChanged(ComponentModel::ListChangedSignalArgs)));
}


void DownloadListModel::connect(const QSharedPointer<DownloadItemViewModel>& item)
{
    QObject::connect(item.data(), SIGNAL(propertyChanged(ComponentModel::PropertyChangedSignalArgs)),
                     this, SLOT(propertyChanged(ComponentModel::PropertyChangedSignalArgs)));
}


void DownloadListModel::connect(QList< QSharedPointer<QObject> >& items)
{
    for (int i = items.count() - 1; i >= 0; --i)
        connect(items[i].staticCast<DownloadItemViewModel>());
}


void DownloadListModel::disconnect(const QWeakPointer<DownloadListViewModel>& list)
{
    for (int i = list.data()->count() - 1; i >= 0; --i)
        disconnect(list.data()->at(i).staticCast<DownloadItemViewModel>());

    QObject::disconnect(list.data(), SIGNAL(listChanged(ComponentModel::ListChangedSignalArgs)));
}


void DownloadListModel::disconnect(const QSharedPointer<DownloadItemViewModel>& item)
{
    QObject::disconnect(item.data(), SIGNAL(propertyChanged(ComponentModel::PropertyChangedSignalArgs)));
}


void DownloadListModel::disconnect(QList< QSharedPointer<QObject> >& items)
{
    for (int i = items.count() - 1; i >= 0; --i)
        disconnect(items[i].staticCast<DownloadItemViewModel>());
}



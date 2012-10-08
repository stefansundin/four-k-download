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


#ifndef DOWNLOADLISTMODEL_H
#define DOWNLOADLISTMODEL_H

#include <QAbstractItemModel>
#include <QWeakPointer>
#include <QMimeData>
#include "viewmodel/downloadlistviewmodel.h"
#include "viewmodel/downloaditemviewmodel.h"

namespace View
{

class DownloadListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QObject* viewModel READ viewModel WRITE setViewModel)
    Q_ENUMS(Role)

public:
    enum Role { TitleRole = Qt::UserRole + 1, DurationRole, SizeRole, ProgressRole, RemainingTimeRole, PlayingTimeRole, StateRole };

    Q_INVOKABLE explicit DownloadListModel(QObject* parent = 0);

    QObject* viewModel() const;
    void setViewModel(QObject* value);

    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex& index, int role) const;
    virtual Qt::ItemFlags flags(const QModelIndex& index) const;
    virtual Qt::DropActions supportedDropActions() const;
    virtual QStringList mimeTypes() const;
    virtual QMimeData* mimeData(const QModelIndexList &indexes) const;
    virtual bool dropMimeData(const QMimeData* data, Qt::DropAction action,
                              int row, int column, const QModelIndex &parent);

protected slots:
    void listChanged(const ComponentModel::ListChangedSignalArgs& args);
    void propertyChanged(const ComponentModel::PropertyChangedSignalArgs& args);

protected:
    virtual void connect(const QWeakPointer<ViewModel::DownloadListViewModel>& list);
    virtual void connect(const QSharedPointer<ViewModel::DownloadItemViewModel>& item);
    virtual void connect(QList< QSharedPointer<QObject> >& items);
    virtual void disconnect(const QWeakPointer<ViewModel::DownloadListViewModel>& list);
    virtual void disconnect(const QSharedPointer<ViewModel::DownloadItemViewModel>& item);
    virtual void disconnect(QList< QSharedPointer<QObject> >& items);

private:
    QWeakPointer<ViewModel::DownloadListViewModel> m_viewModel;
};

} // View

Q_DECLARE_METATYPE(View::DownloadListModel::Role)

#endif // DOWNLOADLISTMODEL_H

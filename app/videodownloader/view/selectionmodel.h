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


#ifndef SELECTIONMODEL_H
#define SELECTIONMODEL_H

#include <QItemSelectionModel>
#include "componentmodel/notifyselection.h"

namespace View
{

class SelectionModel : public QItemSelectionModel
{
    Q_OBJECT
    Q_PROPERTY(QObject* viewModel READ viewModel WRITE setViewModel)

public:
    Q_INVOKABLE SelectionModel(QAbstractItemModel* model, QObject* parent = 0);

    QObject* viewModel() const;
    void setViewModel(QObject* value);

protected slots:
    void selectionChanged(const ComponentModel::SelectionChangedSignalArgs& args);
    void selectionChanged(const QItemSelection& selected, const QItemSelection& deselected);

protected:
    virtual void connect(const QWeakPointer<ComponentModel::NotifySelection>& selection);
    virtual void disconnect(const QWeakPointer<ComponentModel::NotifySelection>& selection);

private:
    QWeakPointer<ComponentModel::NotifySelection> m_viewModel;
};

} // View

#endif // SELECTIONMODEL_H

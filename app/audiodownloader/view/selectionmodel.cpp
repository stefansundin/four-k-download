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


#include "view/selectionmodel.h"

using namespace View;
using namespace ComponentModel;


SelectionModel::SelectionModel(QAbstractItemModel* model, QObject *parent) :
    QItemSelectionModel(model, parent)
{
    QObject::connect(this, SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                     this, SLOT(selectionChanged(QItemSelection,QItemSelection)));
}


QObject* SelectionModel::viewModel() const
{
    return m_viewModel.data();
}


void SelectionModel::setViewModel(QObject* value)
{
    if (m_viewModel)
        disconnect(m_viewModel);

    m_viewModel = dynamic_cast<NotifySelection*>(value);

    if (m_viewModel)
        connect(m_viewModel);

    selectionChanged(SelectionChangedSignalArgs(this, m_viewModel.isNull() ? -1 : m_viewModel.data()->currentIndex()));
}


void SelectionModel::selectionChanged(const SelectionChangedSignalArgs& args)
{
    if (model() && args.currentIndex() >= 0)
    {
        blockSignals(true);
        setCurrentIndex(model()->index(args.currentIndex(), 0), QItemSelectionModel::SelectCurrent);
        blockSignals(false);
    }
}


void SelectionModel::selectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
    Q_UNUSED(deselected)

    int index = selected.isEmpty() ? -1 : selected.at(0).top();

    if (m_viewModel)
        m_viewModel.data()->setCurrentIndex(index);
}


void SelectionModel::connect(const QWeakPointer<ComponentModel::NotifySelection>& selection)
{
    QObject::connect(selection.data(), SIGNAL(selectionChanged(ComponentModel::SelectionChangedSignalArgs)),
                     this, SLOT(selectionChanged(ComponentModel::SelectionChangedSignalArgs)), Qt::QueuedConnection);
}


void SelectionModel::disconnect(const QWeakPointer<ComponentModel::NotifySelection>& selection)
{
    QObject::disconnect(selection.data(), SIGNAL(selectionChanged(ComponentModel::SelectionChangedSignalArgs)));
}

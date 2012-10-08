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


#include "mvvm/cxx/factory.h"
#include <QMetaProperty>
#include <QApplication>

using namespace Mvvm;


Factory::Factory(QObject *parent) :
    QObject(parent),
    m_map(this)
{
}


void Factory::add(const QMetaObject& viewModel, const QMetaObject& view)
{
    m_map.add(viewModel, view);
}


void Factory::remove(const QMetaObject& viewModel)
{
    m_map.remove(viewModel);
}


QObject* Factory::createView(QObject* viewModel) const
{
    QObject* view = m_map.create(viewModel, Q_ARG(const Mvvm::Factory*, this), Q_ARG(QWidget*, QApplication::activeWindow()));

    if (view != NULL)
    {
        int index = view->metaObject()->indexOfProperty("viewModel");
        if (index != -1 && view->metaObject()->property(index).isWritable())
        {
            view->setProperty("viewModel", QVariant::fromValue(viewModel));
        }
    }

    return view;
}

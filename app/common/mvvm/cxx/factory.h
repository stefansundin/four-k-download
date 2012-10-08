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


#ifndef MVVM_CXX_FACTORY_H
#define MVVM_CXX_FACTORY_H

#include <QObject>
#include "componentmodel/metaobjectmap.h"

namespace Mvvm
{

class Factory : public QObject
{
    Q_OBJECT

public:
    explicit Factory(QObject* parent = 0);

    void add(const QMetaObject& viewModel, const QMetaObject& view);
    void remove(const QMetaObject& viewModel);

    QObject* createView(QObject* viewModel) const;

private:
    ComponentModel::MetaObjectMap m_map;
};

} // Mvvm

#endif // MVVM_CXX_FACTORY_H

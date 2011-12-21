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


#ifndef BINDING_H
#define BINDING_H

#include <QObject>
#include <QSharedPointer>
#include "componentmodel/propertychangedsignalargs.h"
#include "componentmodel/binding/converter.h"

namespace Bindings
{

class Binding : public QObject
{
    Q_OBJECT

public:
    enum Mode { TwoWay, OneWay, OneWayToSource };

    Binding(QObject* dstObject, const QString& dstProperty,
            QObject* srcObject, const QString& srcProperty,
            Mode mode = OneWay, Converter* converter = 0, QObject* parent = 0);
    virtual ~Binding();

private slots:
    void propertyChanged();
    void propertyChanged(const ComponentModel::PropertyChangedSignalArgs& args);
    void reset();

protected:
    bool updateDestination() const;
    bool updateSource() const;

private:
    QObject* m_srcObject;
    QObject* m_dstObject;
    QString m_srcProperty;
    QString m_dstProperty;
    Mode m_mode;
    QWeakPointer<Converter> m_converter;
};

} // Bindings

#endif // BINDING_H

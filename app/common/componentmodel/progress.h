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


#ifndef COMPONENTMODEL_PROGRESS_H
#define COMPONENTMODEL_PROGRESS_H

#include <QObject>
#include <QElapsedTimer>
#include <QTimer>
#include "componentmodel/propertychangedsignalargs.h"

namespace ComponentModel
{

class Progress : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qint64 beginValue READ beginValue)
    Q_PROPERTY(qint64 endValue READ endValue)
    Q_PROPERTY(qint64 value READ value WRITE setValue)
    Q_PROPERTY(qint64 elapsedTime READ elapsedTime NOTIFY elapsedTimeChanged)
    Q_PROPERTY(qint64 remainingTime READ remainingTime NOTIFY remainingTimeChanged)

public:
    Progress(qint64 beginValue, qint64 endValue, int updateInterval = 2000, qint64 calcInterval = 0, QObject* parent = 0);

    Q_INVOKABLE bool start();
    Q_INVOKABLE bool stop();
    Q_INVOKABLE void reset();

    qint64 beginValue() const;
    qint64 endValue() const;
    qint64 value() const;
    void setValue(qint64 value);
    qint64 elapsedTime() const;
    qint64 remainingTime() const;

signals:
    void elapsedTimeChanged(const ComponentModel::PropertyChangedSignalArgs& args);
    void remainingTimeChanged(const ComponentModel::PropertyChangedSignalArgs& args);

protected slots:
    void update();

private:
    qint64 m_beginValue;
    qint64 m_endValue;
    qint64 m_value;
    bool m_started;
    qint64 m_calcInterval;
    qint64 m_calcBeginValue;
    qint64 m_calcBeginTime;
    double m_speed;
    qint64 m_elapsedTime;
    QElapsedTimer m_elapsedTimer;
    int m_updateInterval;
    QTimer m_updateTimer;
};

} // ComponentModel

#endif // COMPONENTMODEL_PROGRESS_H

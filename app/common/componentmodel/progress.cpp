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


#include "componentmodel/progress.h"

using namespace ComponentModel;


Progress::Progress(qint64 beginValue, qint64 endValue, int updateInterval, qint64 calcInterval, QObject* parent) :
    QObject(parent),
    m_beginValue(beginValue),
    m_endValue(endValue),
    m_value(beginValue),
    m_started(false),
    m_calcInterval(calcInterval),
    m_calcBeginValue(beginValue),
    m_calcBeginTime(0),
    m_speed(0),
    m_elapsedTime(0),
    m_elapsedTimer(),
    m_updateInterval(updateInterval),
    m_updateTimer()
{
    QObject::connect(&m_updateTimer, SIGNAL(timeout()), this, SLOT(update()));
}


bool Progress::start()
{
    if (m_started)
        return false;

    m_calcBeginValue = value();
    m_calcBeginTime = elapsedTime();

    m_elapsedTimer.start();
    m_updateTimer.start(m_updateInterval);

    m_started = true;

    return true;
}


bool Progress::stop()
{
    if (!m_started)
        return false;

    m_elapsedTime += m_elapsedTimer.elapsed();

    m_elapsedTimer.invalidate();
    m_updateTimer.stop();

    m_started = false;

    return true;
}


void Progress::reset()
{
    m_value = m_beginValue;
    m_elapsedTime = 0;
    m_calcBeginValue = beginValue();
    m_calcBeginTime = 0;
    m_speed = 0;

    m_elapsedTimer.invalidate();
    m_updateTimer.stop();
}


qint64 Progress::beginValue() const
{
    return m_beginValue;
}


qint64 Progress::endValue() const
{
    return m_endValue;
}


qint64 Progress::value() const
{
    return m_value;
}


void Progress::setValue(qint64 value)
{
    if (value < beginValue() || value > endValue())
        return;

    m_value = value;

    if (m_started && m_value - m_calcBeginValue >= m_calcInterval)
    {
        m_speed = 1.0 * (m_value - m_calcBeginValue) / (elapsedTime() - m_calcBeginTime);

        m_calcBeginValue = m_value;
        m_calcBeginTime = elapsedTime();
    }
}


qint64 Progress::elapsedTime() const
{
    return m_started ? m_elapsedTime + m_elapsedTimer.elapsed() : m_elapsedTime;
}


qint64 Progress::remainingTime() const
{
    if (value() == beginValue() || elapsedTime() == 0)
        return -1;

    qint64 time = elapsedTime() * (endValue() - value()) / (value() - beginValue());

    if (m_speed > 0)
    {
        time = 0.7 * time + 0.3 * (endValue() - value()) / m_speed;
    }

    return time;
}


void Progress::update()
{
    emit elapsedTimeChanged(PropertyChangedSignalArgs(this, "elapsedTimeChanged", elapsedTime()));
    emit remainingTimeChanged(PropertyChangedSignalArgs(this, "remainingTimeChanged", remainingTime()));
}

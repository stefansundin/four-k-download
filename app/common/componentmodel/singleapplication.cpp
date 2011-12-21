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


#include <QTimer>
#include <QByteArray>
#include "componentmodel/singleapplication.h"

using namespace ComponentModel;


SingleApplication::SingleApplication(int &argc, char *argv[], const QString uniqueKey) : QApplication(argc, argv)
{
    m_sharedMemory.setKey(uniqueKey);
    if (m_sharedMemory.attach())
        m_isRunning = true;
    else
    {
        m_isRunning = false;
        // attach data to shared memory.
        QByteArray byteArray("0"); // default value to note that no message is available.
        if (!m_sharedMemory.create(byteArray.size()))
        {
            qDebug("Unable to create single instance.");
            return;
        }
        m_sharedMemory.lock();
        char *to = (char*)m_sharedMemory.data();
        const char *from = byteArray.data();
        memcpy(to, from, qMin(m_sharedMemory.size(), byteArray.size()));
        m_sharedMemory.unlock();
        // start checking for messages of other instances.
        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(checkForMessage()));
        timer->start(1000);
    }
}


void SingleApplication::checkForMessage()
{
    m_sharedMemory.lock();
    QByteArray byteArray = QByteArray((char*)m_sharedMemory.constData(), m_sharedMemory.size());
    m_sharedMemory.unlock();
    if (byteArray.left(1) == "0")
        return;
    byteArray.remove(0, 1);
    QString message = QString::fromUtf8(byteArray.constData());
    emit messageAvailable(message);
    // remove message from shared memory.
    byteArray = "0";
    m_sharedMemory.lock();
    char *to = (char*)m_sharedMemory.data();
    const char *from = byteArray.data();
    memcpy(to, from, qMin(m_sharedMemory.size(), byteArray.size()));
    m_sharedMemory.unlock();
}


bool SingleApplication::isRunning()
{
    return m_isRunning;
}


bool SingleApplication::sendMessage(const QString &message)
{
    if (!m_isRunning)
        return false;
    QByteArray byteArray("1");
    byteArray.append(message.toUtf8());
    byteArray.append('\0'); // < should be as char here, not a string!
    m_sharedMemory.lock();
    char *to = (char*)m_sharedMemory.data();
    const char *from = byteArray.data();
    memcpy(to, from, qMin(m_sharedMemory.size(), byteArray.size()));
    m_sharedMemory.unlock();
    return true;
}

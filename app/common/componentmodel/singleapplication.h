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


#ifndef COMPONENTMODEL_SINGLEAPPLICATION_H
#define COMPONENTMODEL_SINGLEAPPLICATION_H

#include <QApplication>
#include <QSharedMemory>

namespace ComponentModel
{

class SingleApplication : public QApplication
{
    Q_OBJECT

public:
    SingleApplication(int &argc, char *argv[], const QString uniqueKey);

    bool isRunning();
    bool sendMessage(const QString &message);

public slots:
    void checkForMessage();

signals:
    void messageAvailable(QString message);

private:
    bool m_isRunning;
    QSharedMemory m_sharedMemory;
};

} // ComponentModel

#endif // COMPONENTMODEL_SINGLE_APPLICATION_H


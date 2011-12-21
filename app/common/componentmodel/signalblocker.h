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


#ifndef SIGNALBLOCKER_H
#define SIGNALBLOCKER_H

#include <QObject>
#include <QWeakPointer>

namespace ComponentModel
{

class SignalBlocker
{
public:
    SignalBlocker(QObject* object);
    virtual ~SignalBlocker();

private:
    QWeakPointer<QObject> m_object;
};

} // ComponentModel

#endif // SIGNALBLOCKER_H

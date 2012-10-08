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


#ifndef COMPONENTMODEL_SELECTIONCHANGEDSIGNALARGS_H
#define COMPONENTMODEL_SELECTIONCHANGEDSIGNALARGS_H

#include "componentmodel/signalargs.h"
#include "componentmodel/metatypes.h"

namespace ComponentModel
{

class SelectionChangedSignalArgs : public SignalArgs
{
public:
    SelectionChangedSignalArgs(QObject* sender = NULL, int currentIndex = -1, int previousIndex = -1);

    int currentIndex() const;
    int previuosIndex() const;

private:
    int m_currentIndex;
    int m_previousIndex;
};

} // ComponentModel

Q_DECLARE_METATYPE(ComponentModel::SelectionChangedSignalArgs)

#endif // COMPONENTMODEL_SELECTIONCHANGEDSIGNALARGS_H

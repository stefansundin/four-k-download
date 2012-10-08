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


#include "gui/cxx/manualtransition.h"

using namespace Gui;


ManualTransitionEvent::ManualTransitionEvent(int id) :
    QEvent(QEvent::Type(QEvent::User+1)),
    id(id)
{
}


ManualTransition::ManualTransition(int id) :
    m_id(id)
{
}


bool ManualTransition::eventTest(QEvent* e)
{
    if (e->type() != QEvent::Type(QEvent::User+1))
        return false;

    ManualTransitionEvent* mte = static_cast<ManualTransitionEvent*>(e);
    return (m_id == mte->id);
}

void ManualTransition::onTransition(QEvent *)
{
}

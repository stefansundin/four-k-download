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


#ifndef COMPONENTMODEL_NOTIFYSELECTION_H
#define COMPONENTMODEL_NOTIFYSELECTION_H

#include <componentmodel/notifylist.h>
#include <componentmodel/selectionchangedsignalargs.h>
#include <QWeakPointer>

namespace ComponentModel
{

class NotifySelection : public QObject
{
    Q_OBJECT

public:
    NotifySelection(NotifyList* list = NULL, QObject* parent = 0);

    NotifyList* list() const;
    void setList(NotifyList* value);

    int currentIndex() const;
    void setCurrentIndex(int value);

    bool isValid() const;

signals:
    void selectionChanged(const ComponentModel::SelectionChangedSignalArgs& args);

protected slots:
    void listChanged(const ComponentModel::ListChangedSignalArgs& args);

protected:
    virtual void connect(const QWeakPointer<NotifyList>& list);
    virtual void disconnect(const QWeakPointer<NotifyList>& list);

private:
    QWeakPointer<NotifyList> m_list;
    int m_currentIndex;
};

} // ComponentModel

#endif // COMPONENTMODEL_NOTIFYSELECTION_H

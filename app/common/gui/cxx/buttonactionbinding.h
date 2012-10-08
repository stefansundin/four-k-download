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


#ifndef GUI_CXX_BUTTONACTIONBINDING_H
#define GUI_CXX_BUTTONACTIONBINDING_H

#include <QObject>
#include <QAction>
#include <QAbstractButton>

namespace Gui
{

class ButtonActionBinding : public QObject
{
    Q_OBJECT

public:
    enum BindOption
    {
        notBindText = 0x1,
        notBindIcon = 0x2,
        notBindVisibility = 0x4
    };
    Q_DECLARE_FLAGS(BindOptions, BindOption)

    ButtonActionBinding(QAbstractButton* button, QAction* action, BindOptions options = 0, QObject* parent = 0);
    virtual ~ButtonActionBinding();

protected slots:
    void buttonClicked(bool checked);
    void actionChanged();
    void reset();

private:
    QAbstractButton* m_button;
    QAction* m_action;
    BindOptions m_options;
};

} // Gui

#endif // GUI_CXX_BUTTONACTIONBINDING_H

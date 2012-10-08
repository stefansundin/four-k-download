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


#ifndef GUI_CXX_LABELACTIONBINDING_H
#define GUI_CXX_LABELACTIONBINDING_H

#include <QWidget>
#include <QAction>
#include <QLabel>

namespace Gui
{

class LabelActionBinding : public QObject
{
    Q_OBJECT

public:
    enum BindOption
    {
        notBindIcon = 0x1,
        notBindVisibility = 0x2
    };
    Q_DECLARE_FLAGS(BindOptions, BindOption)

    LabelActionBinding(QLabel* label, QAction* action, BindOptions options = 0, QObject* parent = 0);
    virtual ~LabelActionBinding();

protected slots:
    void labelClicked();
    void actionChanged();
    void reset();

private:
    QLabel* m_label;
    QAction* m_action;
    BindOptions m_options;
};

} // Gui

#endif // GUI_CXX_LABELACTIONBINDING_H

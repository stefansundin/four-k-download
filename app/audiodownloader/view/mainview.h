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


#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QStateMachine>
#include <QMenu>
#include <QMap>
#include "viewmodel/mainviewmodel.h"
#include "gui/cxx/buttonactionbinding.h"
#include "gui/cxx/labelactionbinding.h"
#include "gui/cxx/component/hoverlistview.h"
#include "view/downloadlistmodel.h"
#include "view/selectionmodel.h"
#include "mvvm/cxx/factory.h"
#include "componentmodel/propertybinding.h"

namespace Ui
{
    class MainView;
}

namespace View
{

class MainView : public QMainWindow
{
    Q_OBJECT
    Q_PROPERTY(QObject* viewModel READ viewModel WRITE setViewModel)
    Q_PROPERTY(int controlAreaWidth READ controlAreaWidth)
    Q_PROPERTY(int controlAreaIndex READ controlAreaIndex)

public:
    Q_INVOKABLE MainView(const Mvvm::Factory* factory, QWidget* parent = 0);
    virtual ~MainView();

    QObject* viewModel() const;
    void setViewModel(QObject* value);
    int controlAreaWidth() const;
    int controlAreaIndex() const;
    void setActiveIndex(const QModelIndex& index);

public slots:
    void notifyUser();

protected slots:
    void layoutControlArea();
    void updateControlArea();
    void updateGreetings();
    void layoutContainers();
    void fillMenu();
    void itemHovered(const QModelIndex& index);
    void itemDoubleClicked(const QModelIndex& index);
    void selectActive();
    void listChanged(const ComponentModel::ListChangedSignalArgs& args);
    void selectionChanged(const ComponentModel::SelectionChangedSignalArgs& args);
    void downloadCompleted();
    void pasteLinkActivated(const QString& url);
    void linkActivated(const QString& url);
    void contextMenuRequest(const QPoint&);

protected:
    void save();
    void restore();

    void showEvent(QShowEvent* event);
    void closeEvent(QCloseEvent* event);
    void resizeEvent(QResizeEvent* event);

    bool eventFilter(QObject *object, QEvent *event);

private:
    Ui::MainView *ui;
    QWeakPointer<const Mvvm::Factory> m_factory;
    QWeakPointer<ViewModel::MainViewModel> m_viewModel;
    DownloadListModel m_list;
    SelectionModel m_selection;
    QModelIndex m_activeIndex;
    QMenu m_menu;
    QScopedPointer<Gui::ButtonActionBinding> m_pasteBinding;
    QScopedPointer<Gui::ButtonActionBinding> m_informationBinding;
    QScopedPointer<Gui::ButtonActionBinding> m_removeBinding;
    QScopedPointer<Gui::ButtonActionBinding> m_clearBinding;
    QScopedPointer<Gui::ButtonActionBinding> m_itemActionBinding;
    QScopedPointer<Gui::ButtonActionBinding> m_itemPlayerActionBinding;
    QStateMachine m_stateMachine;
};

} // View

#endif // MAINVIEW_H

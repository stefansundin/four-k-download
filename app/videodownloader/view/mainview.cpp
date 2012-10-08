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


#include "view/mainview.h"
#include "view/downloaditemdelegate.h"
#include "viewmodel/downloaditemviewmodel.h"
#include "gui/cxx/manualtransition.h"
#include "ui_mainview.h"
#include <QSettings>
#include <QMenu>
#include <QMouseEvent>
#include <QGraphicsOpacityEffect>
#include <QDesktopServices>
#include <QSpacerItem>

using namespace View;
using namespace ViewModel;
using namespace ComponentModel;
using namespace Gui;
using namespace Mvvm;


namespace
{

const int BorderOffset = 6;

} // Anonimous


MainView::MainView(const Factory* factory, QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainView),
    m_factory(factory),
    m_list(this),
    m_selection(&m_list, this),
    m_menu(this)
{
    ui->setupUi(this);

    this->setContextMenuPolicy(Qt::NoContextMenu);

    ui->listView->setFrameShape(QFrame::NoFrame);
    ui->listView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->listView->setItemDelegate(new DownloadItemDelegate(this));
    ui->listView->setModel(&m_list);
    ui->listView->setSelectionModel(&m_selection);
    QObject::connect(ui->listView, SIGNAL(hovered(QModelIndex)),
                     this, SLOT(itemHovered(QModelIndex)));
    QObject::connect(ui->listView, SIGNAL(doubleClicked(QModelIndex)),
                     this, SLOT(itemDoubleClicked(QModelIndex)));
    QObject::connect(ui->listView, SIGNAL(customContextMenuRequested(const QPoint&)),
                     this, SLOT(contextMenuRequest(const QPoint&)));

    QGraphicsOpacityEffect * effect = new QGraphicsOpacityEffect();
    effect->setOpacity(0.5);
    ui->linksWidget->setGraphicsEffect(effect);

#if defined(Q_OS_MAC)
    ui->btnAction->setVerticalPadding(2);
    ui->btnAction->setHorizontalPadding(6);
    ui->btnAction->setCursor(Qt::PointingHandCursor);
#else
    ui->btnAction->setVerticalPadding(3);
    ui->btnAction->setHorizontalPadding(6);
    QFont font = ui->btnAction->font();
    font.setBold(true);
    ui->btnAction->setFont(font);
#endif
    ui->btnAction->setParent(ui->listView->viewport());
    ui->btnAction->hide();
    QObject::connect(ui->btnAction, SIGNAL(pressed()),
                     this, SLOT(selectActive()));

#if defined(Q_OS_MAC)
    ui->btnRemove->setCursor(Qt::PointingHandCursor);
#endif
    ui->btnRemove->setVerticalPadding(2);
    ui->btnRemove->setHorizontalPadding(0);
    ui->btnRemove->setParent(ui->listView->viewport());
    ui->btnRemove->hide();
    QObject::connect(ui->btnRemove, SIGNAL(pressed()),
                     this, SLOT(selectActive()));

    ui->greetingsWidget->setParent(ui->listView->viewport());


    QObject::connect(ui->greetingsLabel, SIGNAL(linkActivated(QString)),
                     this, SLOT(pasteLinkActivated(QString)));
    QObject::connect(ui->youtubeLabel, SIGNAL(linkActivated(QString)),
                     this, SLOT(linkActivated(QString)));
    QObject::connect(ui->facebookLabel, SIGNAL(linkActivated(QString)),
                     this, SLOT(linkActivated(QString)));
    QObject::connect(ui->vimeoLabel, SIGNAL(linkActivated(QString)),
                     this, SLOT(linkActivated(QString)));
    QObject::connect(ui->dailymotionLabel, SIGNAL(linkActivated(QString)),
                     this, SLOT(linkActivated(QString)));

    QObject::connect(&m_menu, SIGNAL(aboutToShow()), this, SLOT(fillMenu()));


    restore();
}

MainView::~MainView()
{
    delete ui;
}


void MainView::save()
{
    QSettings settings;
    settings.setValue("geometry", saveGeometry());
    settings.setValue("state", saveState());
}


void MainView::restore()
{
    QSettings settings;
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("state").toByteArray());
}


QObject* MainView::viewModel() const
{
    return m_viewModel.data();
}


void MainView::setViewModel(QObject* value)
{
    if (m_viewModel.data() != value)
    {
        if (m_viewModel)
        {
            m_list.setViewModel(NULL);
            m_selection.setViewModel(NULL);

            ui->toolBar->removeAction(m_viewModel.data()->pasteAction());
            ui->toolBar->removeAction(m_viewModel.data()->smartModeAction());
            ui->toolBar->removeAction(m_viewModel.data()->informationAction());
            ui->toolBar->removeAction(m_viewModel.data()->facebookAction());
            ui->toolBar->clear();
            m_removeBinding.reset();

            removeAction(m_viewModel.data()->pasteAction());
            removeAction(m_viewModel.data()->smartModeAction());
            removeAction(m_viewModel.data()->informationAction());
            removeAction(m_viewModel.data()->removeAction());
            removeAction(m_viewModel.data()->clearAction());

            QObject::disconnect(m_viewModel.data()->list(), SIGNAL(listChanged(ComponentModel::ListChangedSignalArgs)));
            QObject::disconnect(m_viewModel.data()->selection(), SIGNAL(selectionChanged(ComponentModel::SelectionChangedSignalArgs)));
            QObject::disconnect(m_viewModel.data(), SIGNAL(downloadCompleted()));
        }


        m_viewModel = dynamic_cast<MainViewModel*>(value);


        if (m_viewModel && m_factory)
        {
            m_list.setViewModel(m_viewModel.data()->list());
            m_selection.setViewModel(m_viewModel.data()->selection());

            ui->toolBar->addAction(m_viewModel.data()->pasteAction());
            ui->toolBar->addAction(m_viewModel.data()->smartModeAction());
            ui->toolBar->addSeparator();
            ui->toolBar->addAction(m_viewModel.data()->informationAction());
            QWidget* spacer = new QWidget();
            spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
            ui->toolBar->addWidget(spacer);
            ui->toolBar->addAction(m_viewModel.data()->facebookAction());
            m_removeBinding.reset(new ButtonActionBinding(ui->btnRemove, m_viewModel.data()->removeAction(),
                                  (ButtonActionBinding::BindOptions)ButtonActionBinding::notBindIcon | ButtonActionBinding::notBindText | ButtonActionBinding::notBindVisibility));

            addAction(m_viewModel.data()->pasteAction());
            addAction(m_viewModel.data()->smartModeAction());
            addAction(m_viewModel.data()->informationAction());
            addAction(m_viewModel.data()->removeAction());
            addAction(m_viewModel.data()->clearAction());

            QObject::connect(m_viewModel.data()->list(), SIGNAL(listChanged(ComponentModel::ListChangedSignalArgs)),
                             this, SLOT(listChanged(ComponentModel::ListChangedSignalArgs)));
            QObject::connect(m_viewModel.data()->selection(), SIGNAL(selectionChanged(ComponentModel::SelectionChangedSignalArgs)),
                             this, SLOT(selectionChanged(ComponentModel::SelectionChangedSignalArgs)), Qt::QueuedConnection);
            QObject::connect(m_viewModel.data(), SIGNAL(downloadCompleted()),
                             this, SLOT(downloadCompleted()));
        }
    }
}


int MainView::controlAreaWidth() const
{
    return ui->btnAction->width() + 2 * BorderOffset;
}


int MainView::controlAreaIndex() const
{
    if (m_activeIndex.isValid())
        return m_activeIndex.row();

    return -1;
}


void MainView::setActiveIndex(const QModelIndex& index)
{
    if (m_activeIndex != index)
    {
        QModelIndex prevIndex = m_activeIndex;
        m_activeIndex = index;

        ui->listView->update(prevIndex);
        ui->listView->update(m_activeIndex);

        updateControlArea();
    }
}


void MainView::itemHovered(const QModelIndex& index)
{
    setActiveIndex(index);
}


void MainView::itemDoubleClicked(const QModelIndex& index)
{
    if (!m_viewModel || !index.isValid())
        return;

    m_viewModel.data()->list()->at(index.row()).staticCast<DownloadItemViewModel>()->play();
}


void MainView::selectActive()
{
    m_selection.setCurrentIndex(m_activeIndex, QItemSelectionModel::ClearAndSelect);
}


void MainView::listChanged(const ComponentModel::ListChangedSignalArgs& args)
{
    Q_UNUSED(args)

    if (ComponentModel::ListChangedSignalArgs::Insert == args.action())
        ui->listView->scrollToTop();

    updateGreetings();
}


void MainView::selectionChanged(const ComponentModel::SelectionChangedSignalArgs& args)
{
    Q_UNUSED(args)

    QMouseEvent event(QEvent::MouseMove, ui->listView->mapFromGlobal(QCursor::pos()), (Qt::MouseButton)0, (Qt::MouseButtons)0, 0);
    QApplication::sendEvent(ui->listView->viewport(), &event);

    updateControlArea();
}


void MainView::downloadCompleted()
{
    QApplication::alert(this);
}


void MainView::pasteLinkActivated(const QString& url)
{
    if (m_viewModel && m_viewModel.data()->pasteAction()->isEnabled())
        m_viewModel.data()->pasteAction()->trigger();
}


void MainView::linkActivated(const QString& url)
{
    QDesktopServices::openUrl(QUrl(url));
}


void MainView::contextMenuRequest(const QPoint& point)
{
    m_menu.exec(ui->listView->mapToGlobal(point));
}


void MainView::layoutControlArea()
{
    if (m_activeIndex.isValid())
    {
        QPalette pallete = ui->btnAction->palette();
        if (ui->listView->hasFocus() && m_selection.hasSelection() &&
            m_selection.selectedRows().at(0).row() == m_activeIndex.row())
            pallete.setColor(QPalette::ButtonText, palette().brightText().color());
        else
            pallete.setColor(QPalette::ButtonText, palette().buttonText().color());
        ui->btnAction->setPalette(pallete);

        QRect itemRect = ui->listView->visualRect(m_activeIndex);

        QSize btnSize = ui->btnAction->minimumSizeHint();
        ui->btnAction->setGeometry(itemRect.left() + itemRect.width() - btnSize.width() - BorderOffset, itemRect.bottom() - btnSize.height() - 0.7 * BorderOffset,
                                    btnSize.width(), btnSize.height());

        QRect btnRect = QRect(QPoint(0, 0), ui->btnRemove->minimumSizeHint());
        btnRect.moveLeft(itemRect.left() + itemRect.width() - btnRect.width() - BorderOffset);
        btnRect.moveTop(itemRect.top() + 0.7 * BorderOffset);
        ui->btnRemove->setGeometry(btnRect);
    }
}


void MainView::updateControlArea()
{
    if (m_activeIndex.isValid())
    {
        QAction* action = m_viewModel.data()->list()->at(m_activeIndex.row()).staticCast<DownloadItemViewModel>()->basicAction();
        m_itemActionBinding.reset(new ButtonActionBinding(ui->btnAction, action, (ButtonActionBinding::BindOptions)ButtonActionBinding::notBindIcon));
        QObject::connect(action, SIGNAL(changed()), this, SLOT(layoutControlArea()));

        layoutControlArea();

        ui->btnAction->show();
        ui->btnRemove->show();
    }
    else
    {
        ui->btnAction->hide();
        ui->btnRemove->hide();

        m_itemActionBinding.reset();
    }
}


void MainView::updateGreetings()
{
    ui->greetingsWidget->setVisible(m_viewModel.data()->list()->count() == 0);
    ui->greetingsWidget->setGeometry(QRect(0, 0, ui->listView->width(), ui->listView->height() * 0.95));
}


void MainView::fillMenu()
{
    m_menu.clear();


    if (!m_viewModel)
        return;


    QList<QAction*> list;

    if (m_selection.currentIndex().isValid())
    {
        QAction* action = m_viewModel.data()->list()->at(m_selection.currentIndex().row()).staticCast<DownloadItemViewModel>()->basicAction();
        if (action->isEnabled())
            list.append(action);
        action = m_viewModel.data()->list()->at(m_selection.currentIndex().row()).staticCast<DownloadItemViewModel>()->folderAction();
        if (action->isEnabled())
            list.append(action);
        action = m_viewModel.data()->list()->at(m_selection.currentIndex().row()).staticCast<DownloadItemViewModel>()->urlAction();
        list.append(action);
        m_menu.addActions(list);
        m_menu.addSeparator();
        list.clear();
        action = m_viewModel.data()->list()->at(m_selection.currentIndex().row()).staticCast<DownloadItemViewModel>()->facebookAction();
        list.append(action);
        action = m_viewModel.data()->list()->at(m_selection.currentIndex().row()).staticCast<DownloadItemViewModel>()->twitterAction();
        list.append(action);
        m_menu.addActions(list);
        m_menu.addSeparator();
        list.clear();
    }
    list.append(m_viewModel.data()->pauseAllAction());
    list.append(m_viewModel.data()->resumeAllAction());
    m_menu.addActions(list);
    m_menu.addSeparator();
    list.clear();
    list.append(m_viewModel.data()->pasteAction());
    list.append(m_viewModel.data()->removeAction());
    list.append(m_viewModel.data()->clearAction());
    m_menu.addActions(list);
}


void MainView::showEvent(QShowEvent* event)
{
    Q_UNUSED(event)
    layoutContainers();
    updateGreetings();
}


void MainView::closeEvent(QCloseEvent* event)
{
    if (m_viewModel && !m_viewModel.data()->canClose())
    {
        event->ignore();
        return;
    }

    save();
}


void MainView::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);

    layoutContainers();
    updateGreetings();

    if (m_viewModel)
        layoutControlArea();
}


void MainView::layoutContainers()
{
    QSize size = ui->centralWidget->size();

    QRect listRect = QRect(0, 0, size.width(), size.height());

    ui->listView->setGeometry(listRect);

    update();
}


void MainView::notifyUser()
{
    raise();
    activateWindow();
}


bool MainView::eventFilter(QObject *object, QEvent *event)
{
    if (!m_viewModel)
        return false;

    return false;
}

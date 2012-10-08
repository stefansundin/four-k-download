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


#include "mvvm/qml/dialog.h"
#include "componentmodel/proxyimageprovider.h"
#include "componentmodel/propertybinding.h"
#include <QDeclarativeContext>
#include <QDeclarativeEngine>
#include <QDeclarativeView>
#include <QDeclarativeListProperty>
#include <QDeclarativeItem>
#include <QSettings>
#include <QVariant>
#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopServices>
#include <QApplication>
#include <QGridLayout>
#include <QGraphicsObject>
#include <QMetaMethod>
#include <QMainWindow>
#include <QToolBar>
#include <QMenuBar>
#include <QDesktopWidget>

using namespace Mvvm;
using namespace ComponentModel;


Q_DECLARE_METATYPE(QDeclarativeImageProvider*)


static QString openDialogDirectory;
static QString saveDialogDirectory;
static QString dirDialogDirectory;
static QByteArray mainWindowGeometry;
static QStringList importPaths;
static QStringList pluginPaths;



Dialog::Dialog(QObject *parent) :
    QObject(parent)
{
    QSettings settings;
    QVariant defaultDirectory = QVariant::fromValue(QDesktopServices::storageLocation(QDesktopServices::MoviesLocation));
    openDialogDirectory = settings.value("Dialog/openDialogDirectory", defaultDirectory).toString();
    saveDialogDirectory = settings.value("Dialog/saveDialogDirectory", defaultDirectory).toString();
    dirDialogDirectory = settings.value("Dialog/dirDialogDirectory", dirDialogDirectory).toString();
    mainWindowGeometry = settings.value("Dialog/mainWindowGeometry", mainWindowGeometry).toByteArray();
}


Dialog::~Dialog()
{
    QSettings settings;
    if (!openDialogDirectory.isEmpty())
        settings.setValue("Dialog/openDialogDirectory", openDialogDirectory);
    if (!saveDialogDirectory.isEmpty())
        settings.setValue("Dialog/saveDialogDirectory", saveDialogDirectory);
    if (!dirDialogDirectory.isEmpty())
        settings.setValue("Dialog/dirDialogDirectory", dirDialogDirectory);
    if (!mainWindowGeometry.isEmpty())
        settings.setValue("Dialog/mainWindowGeometry", mainWindowGeometry);
}


int Dialog::showDialog(const QString& view, int type)
{
    return showDialog(view, NULL, type);
}


int Dialog::showDialog(const QString& view, QObject* viewModel, int type)
{
    QDialog* dialog = NULL;
    QMainWindow* mainWindow = NULL;
    QWidget* windowWidget = NULL;
    QWidget* layoutWidget = NULL;

    switch (type)
    {
    case Dialog::MainWindow:
        mainWindow = new QMainWindow();
        windowWidget = mainWindow;
        layoutWidget = new QWidget(windowWidget);
        mainWindow->setCentralWidget(layoutWidget);
        break;

    case Dialog::ModalDialog:
        dialog = new QDialog(QApplication::activeWindow());
        windowWidget = dialog;
        layoutWidget = dialog;
        break;

    default:
        dialog = new QDialog();
        windowWidget = dialog;
        layoutWidget = dialog;
        break;
    }

    QGridLayout* layout = new QGridLayout(layoutWidget);

    // Create view

    QDeclarativeView* v = new QDeclarativeView(layoutWidget);

    if (viewModel)
    {
        int count = viewModel->metaObject()->propertyCount();
        for (int i = 0; i < count; ++i)
        {
            QMetaProperty p = viewModel->metaObject()->property(i);
            if (p.isReadable() && p.typeName() == QString("QDeclarativeImageProvider*"))
            {
                QString name = p.name();
                QDeclarativeImageProvider* value = p.read(viewModel).value<QDeclarativeImageProvider*>();

                v->engine()->addImageProvider(name.toLatin1(), new ProxyImageProvider(value));
            }
        }

        v->rootContext()->setContextProperty("dataContext", viewModel);
    }

    QString path;
    foreach (path, importPaths)
        v->engine()->addImportPath(path);
    foreach (path, pluginPaths)
        v->engine()->addPluginPath(path);

    v->setSource(QUrl(view));
    v->setResizeMode(QDeclarativeView::SizeRootObjectToView);

    // Initialize dialog

    QGraphicsObject* root = v->rootObject();

    QVariant property = root->property("dialogTitle");
    if (property.isValid())
        windowWidget->setWindowTitle(property.toString());

    property = root->property("dialogMinWidth");
    if (property.isValid())
        layoutWidget->setMinimumWidth(property.toInt());

    property = root->property("dialogMinHeight");
    if (property.isValid())
        layoutWidget->setMinimumHeight(property.toInt());

    property = root->property("dialogMaxWidth");
    if (property.isValid())
        layoutWidget->setMaximumWidth(property.toInt());

    property = root->property("dialogMaxHeight");
    if (property.isValid())
        layoutWidget->setMaximumHeight(property.toInt());

    property = root->property("dialogResizable");
    if (property.isValid() && !property.toBool())
        layout->setSizeConstraint(QLayout::SetFixedSize);

    Qt::WindowStates states = windowWidget->windowState();
    Qt::WindowFlags flags = windowWidget->windowFlags();

    property = root->property("dialogMinimizeButton");
    if (property.isValid())
        flags = property.toBool() ? flags | Qt::WindowMinimizeButtonHint : flags & ~Qt::WindowMinimizeButtonHint;

    property = root->property("dialogMaximizeButton");
    if (property.isValid())
        flags = property.toBool() ? flags | Qt::WindowMaximizeButtonHint : flags & ~Qt::WindowMaximizeButtonHint;

    property = root->property("dialogCloseButton");
    if (property.isValid())
        flags = property.toBool() ? flags | Qt::WindowCloseButtonHint : flags & ~Qt::WindowCloseButtonHint;

    property = root->property("dialogFullScreen");
    if (property.isValid())
        states = property.toBool() ? states | Qt::WindowFullScreen : states & ~Qt::WindowFullScreen;

    flags = flags & ~Qt::WindowContextHelpButtonHint;

    windowWidget->setWindowFlags(flags);
    windowWidget->setWindowState(states);

    property = root->property("dialogToolBar");
    if (type == Dialog::MainWindow && property.isValid() && property.typeName() == QString("QDeclarativeListProperty<QDeclarativeItem>"))
    {
        QToolBar* toolbar = new QToolBar(mainWindow);
        toolbar->setMovable(false);
        toolbar->setFloatable(false);
        toolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        toolbar->setAllowedAreas(Qt::TopToolBarArea);

        QDeclarativeListProperty<QDeclarativeItem> btnList = property.value< QDeclarativeListProperty<QDeclarativeItem> >();
        int btnCount = btnList.count(&btnList);

        for (int i = 0; i < btnCount; ++i)
        {
            QDeclarativeItem* item = btnList.at(&btnList, i);

            if (!item->property("text").isValid())
                continue;

            QString itemText = item->property("text").toString();
            QString itemTooltip = item->property("tooltip").isValid() ? item->property("tooltip").toString() : "";
            QString itemIconSource = item->property("iconSource").isValid() ? item->property("iconSource").toString() : "";
            int itemIconSize = item->property("iconSize").isValid() ? item->property("iconSize").toInt() : -1;

            if (itemText == "|")
            {
                toolbar->addSeparator();
            }
            else if (itemText == "-")
            {
                QWidget* spacer = new QWidget();
                spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
                toolbar->addWidget(spacer);
            }
            else
            {
                QAction* action = new QAction(mainWindow);
                action->setText(itemText);
                action->setToolTip(itemTooltip);
                action->setIcon(QIcon(itemIconSource));
                QObject::connect(action, SIGNAL(triggered()), item, SLOT(trigger()));

                if (item->property("enabled").isValid())
                    new PropertyBinding(action, "enabled", item, "enabled", PropertyBinding::OneWay, NULL, this);

                if (item->property("visible").isValid())
                    new PropertyBinding(action, "visible", item, "visible", PropertyBinding::OneWay, NULL, this);

                toolbar->addAction(action);
            }

            if (itemIconSize != -1)
                toolbar->setIconSize(QSize(itemIconSize, itemIconSize));
        }

        mainWindow->setUnifiedTitleAndToolBarOnMac(true);
        mainWindow->addToolBar(toolbar);
    }

    property = root->property("dialogMenu");
    if (type == Dialog::MainWindow && property.isValid() && property.typeName() == QString("QDeclarativeListProperty<QDeclarativeItem>"))
    {
        QDeclarativeListProperty<QDeclarativeItem> list = property.value< QDeclarativeListProperty<QDeclarativeItem> >();
        int count = list.count(&list);

        for (int i = 0; i < count; ++i)
        {
            QDeclarativeItem* item = list.at(&list, i);

            if (!item->property("text").isValid())
                continue;

            QString itemText = item->property("text").toString();
            QMenu * menuItem = mainWindow->menuBar()->addMenu(itemText);

            if (!item->property("submenu").isValid() || item->property("submenu").typeName() != QString("QDeclarativeListProperty<QDeclarativeItem>"))
                continue;

            QDeclarativeListProperty<QDeclarativeItem> innerList = item->property("submenu").value< QDeclarativeListProperty<QDeclarativeItem> >();
            int innerCount = innerList.count(&innerList);

            for (int j = 0; j < innerCount; ++j)
            {
                QDeclarativeItem* innerItem = innerList.at(&innerList, j);

                if (!innerItem->property("text").isValid())
                    continue;

                QString innerItemText = innerItem->property("text").toString();
                QString innerItemShortcut = innerItem->property("shortcut").isValid() ? innerItem->property("shortcut").toString() : "";
                QString innerItemIconSource = innerItem->property("iconSource").isValid() ? innerItem->property("iconSource").toString() : "";

                if (innerItemText == "-")
                {
                    menuItem->addSeparator();
                }
                else
                {
                    QAction * action = menuItem->addAction(QIcon(innerItemIconSource), innerItemText);
                    action->setShortcut(QKeySequence(innerItemShortcut));

                    QObject::connect(action, SIGNAL(triggered()), innerItem, SLOT(trigger()));

                    if (innerItem->property("enabled").isValid())
                        new PropertyBinding(action, "enabled", innerItem, "enabled", PropertyBinding::OneWay, NULL, this);

                    if (innerItem->property("visible").isValid())
                        new PropertyBinding(action, "visible", innerItem, "visible", PropertyBinding::OneWay, NULL, this);
                }
            }
        }
    }

    new DialogCallbacks(windowWidget, v, root);

    // Initialize layout

    layout->setMargin(0);
    layout->addWidget(v, 0, 0);

    // Execute

    switch (type)
    {
    case Dialog::ModalDialog:
        dialog->exec();
        break;

    case Dialog::MainWindow:
    {
        if (mainWindowGeometry.isEmpty())
        {
            mainWindow->adjustSize();
            mainWindow->move(QApplication::desktop()->screen()->rect().center() - mainWindow->rect().center());
        }
        else
            mainWindow->restoreGeometry(mainWindowGeometry);
    }

    default:
        windowWidget->setAttribute(Qt::WA_DeleteOnClose);
        windowWidget->show();
        break;
    }

    int result = 0;

    property = root->property("dialogResult");
    if (property.isValid())
        result = property.toInt();

    if (type == Dialog::ModalDialog)
        delete dialog;

    return result;
}


QStringList Dialog::showOpenDialog(bool singleSelect, const QString& filters, const QString& directory, const QString& caption) const
{
    QStringList result;

    if (singleSelect)
    {
        QString filename = QFileDialog::getOpenFileName(QApplication::activeWindow(),
                                                        caption.isEmpty() ? QCoreApplication::applicationName() : caption,
                                                        directory.isEmpty() ? openDialogDirectory : directory,
                                                        filters, NULL, QFlags<QFileDialog::Option>());
        if (!filename.isEmpty())
            result << filename;
    }
    else
    {
        result = QFileDialog::getOpenFileNames(QApplication::activeWindow(),
                                               caption.isEmpty() ? QCoreApplication::applicationName() : caption,
                                               directory.isEmpty() ? openDialogDirectory : directory,
                                               filters, NULL, QFlags<QFileDialog::Option>());
    }

    if (!result.isEmpty())
        openDialogDirectory = QFileInfo(result.at(0)).canonicalPath();

    return result;
}


QString Dialog::showSaveDialog(const QString& filters, const QString& directory, const QString& caption) const
{
    QString result = QFileDialog::getSaveFileName(QApplication::activeWindow(),
                                                  caption.isEmpty() ? QCoreApplication::applicationName() : caption,
                                                  directory.isEmpty() ? saveDialogDirectory : directory,
                                                  filters, NULL, QFlags<QFileDialog::Option>());

    if (!result.isEmpty())
        saveDialogDirectory = QFileInfo(result).canonicalPath();

    return result;
}


QString Dialog::showDirectoryDialog(const QString& directory, const QString& caption) const
{
    QString result = QFileDialog::getExistingDirectory(QApplication::activeWindow(),
                                                       caption.isEmpty() ? QCoreApplication::applicationName() : caption,
                                                       directory.isEmpty() ? dirDialogDirectory : directory,
                                                       QFileDialog::ShowDirsOnly);

    if (!result.isEmpty())
        dirDialogDirectory = QDir(result).canonicalPath();

    return result;
}


int Dialog::showMessageDialog(const QString& text, int icon, const QString& caption, const QStringList& buttons, int defaultIndex) const
{
    QMessageBox messageBox((QMessageBox::Icon)icon, caption.isEmpty() ? QCoreApplication::applicationName() : caption,
                           text, QMessageBox::NoButton, QApplication::activeWindow());
    QList<QAbstractButton*> buttonList;

    for (int i = 0; i < buttons.count(); ++i)
    {
        buttonList.append(
            (QAbstractButton*)messageBox.addButton(buttons[i], i == defaultIndex ? QMessageBox::AcceptRole : QMessageBox::RejectRole)
        );
    }

    if (defaultIndex >= 0 && defaultIndex < buttonList.count())
        messageBox.setDefaultButton((QPushButton*)buttonList[defaultIndex]);

    messageBox.exec();
    return buttonList.indexOf(messageBox.clickedButton());
}


void Dialog::addImportPath(QString path)
{
    importPaths.append(path);
}


void Dialog::addPluginPath(QString path)
{
    pluginPaths.append(path);
}



DialogCallbacks::DialogCallbacks(QWidget* dialog, QDeclarativeView* declarative, QObject* view) :
    QObject(dialog),
    m_dialog(dialog),
    m_declarative(declarative),
    m_view(view)
{
    int index = view->metaObject()->indexOfProperty("dialogFullScreen");
    if (index != -1)
    {
        QMetaProperty metaProperty = view->metaObject()->property(index);
        if (metaProperty.hasNotifySignal())
        {
            QMetaObject::connect(view, metaProperty.notifySignalIndex(),
                                 this, metaObject()->indexOfSlot("viewFullScreenChanged()"));
        }
    }

    index = view->metaObject()->indexOfSignal("dialogCloseRequested()");
    if (index != -1)
        QObject::connect(view, SIGNAL(dialogCloseRequested()), dialog, SLOT(close()));

    index = view->metaObject()->indexOfProperty("width");
    if (index != -1)
    {
        QMetaProperty metaProperty = view->metaObject()->property(index);
        if (metaProperty.hasNotifySignal())
        {
            QMetaObject::connect(view, metaProperty.notifySignalIndex(),
                                 this, metaObject()->indexOfSlot("widthChanged()"));
        }
    }

    index = view->metaObject()->indexOfProperty("height");
    if (index != -1)
    {
        QMetaProperty metaProperty = view->metaObject()->property(index);
        if (metaProperty.hasNotifySignal())
        {
            QMetaObject::connect(view, metaProperty.notifySignalIndex(),
                                 this, metaObject()->indexOfSlot("heightChanged()"));
        }
    }

    dialog->installEventFilter(this);
}


void DialogCallbacks::viewFullScreenChanged()
{
    QVariant property = m_view->property("dialogFullScreen");
    if (!property.isValid())
        return;

    if (property.toBool())
        m_dialog->showFullScreen();
    else
        m_dialog->showNormal();
}


void DialogCallbacks::widthChanged()
{
    QVariant property = m_view->property("width");
    if (!property.isValid())
        return;

    m_declarative->resize(property.toInt(), m_declarative->height());
}


void DialogCallbacks::heightChanged()
{
    QVariant property = m_view->property("height");
    if (!property.isValid())
        return;

    m_declarative->resize(m_declarative->width(), property.toInt());
}


bool DialogCallbacks::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::Close)
    {
        int index = m_view->metaObject()->indexOfMethod("dialogCanClose()");
        if (index != -1)
        {
            QVariant returnedValue; returnedValue.setValue<bool>(true);
            QMetaMethod metaMethod = m_view->metaObject()->method(index);
            metaMethod.invoke(m_view, Q_RETURN_ARG(QVariant, returnedValue));

            if (!returnedValue.toBool())
            {
                event->ignore();
                return true;
            }
        }

        QMainWindow* mainWindow = qobject_cast<QMainWindow*>(object);
        if (mainWindow)
            mainWindowGeometry = mainWindow->saveGeometry();
    }

    return false;
}

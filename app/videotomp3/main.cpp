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


#include <QtGui/QApplication>
#include <QMetaMethod>
#include <QMetaProperty>
#include <QVariant>
#include <QSettings>
#include <QTranslator>
#include <QLibraryInfo>
#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QIcon>
#include <QLocale>
#include "viewmodel/mainviewmodel.h"
#include "componentmodel/singleapplication.h"
#include "componentmodel/platform.h"
#include "componentmodel/transform.h"
#include "gui/qml/droparea.h"
#include "mvvm/qml/dialog.h"

#if defined(Q_OS_WIN)
#include <windows.h>
#endif


using namespace ComponentModel;
using namespace ViewModel;
using namespace Mvvm;


int main(int argc, char *argv[])
{
    // Prevent installation

#if defined(Q_OS_WIN)

    CreateMutex(NULL, FALSE, L"4KVIDEOTOMP3");

#endif

    // Prevent multiple execution

    ComponentModel::SingleApplication a(argc, argv, "4KVIDEOTOMP3");

    if (a.isRunning())
    {
        a.sendMessage("show");
        return 0;
    }

    // Libraries

    Q_INIT_RESOURCE(guiqml);

#if defined(Q_OS_WIN)

    QDir libraryDir(QApplication::applicationDirPath());

#elif defined(Q_OS_LINUX)

    QDir libraryDir(QApplication::applicationDirPath());

#elif defined(Q_OS_MAC)

    QDir libraryDir(QApplication::applicationDirPath());
    libraryDir.cdUp();
    libraryDir.cd("PlugIns");

#endif

    QApplication::addLibraryPath(libraryDir.absolutePath());

    // Initialize

    QApplication::setApplicationVersion(APP_VERSION);
    QApplication::setOrganizationName("4kdownload.com");
    QApplication::setApplicationName("4K Video to MP3");
    QApplication::setWindowIcon(QIcon(":/image/application-icon"));

    QSettings settings;
    settings.setValue("mailSupport", "mailto:support@4kdownload.com");
    settings.setValue("web", "http://www.4kdownload.com?source=4k-video-to-mp3");
    settings.setValue("webHelp", "http://www.4kdownload.com/howto/howto-convert-video-to-mp3?source=4k-video-to-mp3");
    settings.setValue("webThanks", "http://www.4kdownload.com/thanks-for-installing?source=4k-video-to-mp3");
    settings.setValue("webFacebook", "http://www.facebook.com/4kdownload");
    settings.setValue("socialUrl", "http://www.4kdownload.com/");
    settings.setValue("socialReferrer", "http://www.4kdownload.com/");
    settings.setValue("socialVia", "4kdownload");

    // Translate

    QTranslator qtTranslator;
    QTranslator appTranslator;

#if defined(Q_OS_WIN)

    QDir translationDir(QApplication::applicationDirPath());
    translationDir.cd("translation");

#elif defined(Q_OS_LINUX)

    QDir translationDir(QApplication::applicationDirPath());
    translationDir.cd("translation");

#elif defined(Q_OS_MAC)

    QDir translationDir(QApplication::applicationDirPath());
    translationDir.cdUp();
    translationDir.cd("Resources/translation");

#endif

    qtTranslator.load("qt_" + QLocale::system().name(), translationDir.absolutePath());

    if (!appTranslator.load("videotomp3_" + QLocale::system().name().left(2), translationDir.absolutePath()))
        appTranslator.load("videotomp3_en", translationDir.absolutePath());

    a.installTranslator(&qtTranslator);
    a.installTranslator(&appTranslator);

    // Register QML types

    qmlRegisterType<ComponentModel::Transform>("ComponentModel", 1, 0, "Transform");
    qmlRegisterType<ComponentModel::Platform>("ComponentModel", 1, 0, "Platform");
    qmlRegisterUncreatableType<MediaItemViewModel>("Application", 1, 0, "MediaItemViewModel", "Cant'create MediaItemViewModel!");

    // Execute

    MainViewModel viewModel;

    Dialog dialog;
    dialog.showDialog("qrc:/qml/mainview", &viewModel, Dialog::MainWindow);

    QObject::connect(&a, SIGNAL(messageAvailable(QString)), &viewModel, SLOT(alert()));

    return a.exec();
}

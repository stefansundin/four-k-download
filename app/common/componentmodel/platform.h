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


#ifndef COMPONENTMODEL_PLATFORM_H
#define COMPONENTMODEL_PLATFORM_H

#include <QObject>
#include <QtGlobal>
#include <QMetaType>
#include <QSysInfo>
#include "componentmodel/metatypes.h"

namespace ComponentModel
{

class Platform : public QObject
{
    Q_OBJECT
    Q_ENUMS(Architecture WindowsVersion MacVersion)

public:
    enum Architecture { UnknownArchitecture, x86, x64 };
    enum WindowsVersion { UnknownWindowsVersion = 0, WindowsXP, WindowsVista, Windows7 };
    enum MacVersion { UnknownMacVersion = 0, Mac10_5, Mac10_6, Mac10_7 };

    Q_INVOKABLE static bool isWindows();
    Q_INVOKABLE static bool isMac();
    Q_INVOKABLE static bool isLinux();
    Q_INVOKABLE static bool isUbuntu();
    Q_INVOKABLE static bool isDebian();
    Q_INVOKABLE static bool isFedora();

    Q_INVOKABLE static WindowsVersion windowsVersion();
    Q_INVOKABLE static MacVersion macVersion();
    Q_INVOKABLE static Architecture architecture();
    Q_INVOKABLE static QString os();
    Q_INVOKABLE static QString platform();
};

} // Plarform

Q_DECLARE_METATYPE(ComponentModel::Platform::Architecture)

#endif // COMPONENTMODEL_PLATFORM_H

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


#include "componentmodel/platform.h"
#include <QFileInfo>

using namespace ComponentModel;


bool Platform::isWindows()
{
#if defined(Q_OS_WIN)
    return true;
#else
    return false;
#endif
}


bool Platform::isMac()
{
#if defined(Q_OS_MAC)
    return true;
#else
    return false;
#endif
}


bool Platform::isLinux()
{
#if defined(Q_OS_LINUX)
    return true;
#else
    return false;
#endif
}


bool Platform::isUbuntu()
{
    if (isLinux())
    {
        QFileInfo info("/etc/lsb-release");
        return info.exists();
    }

    return false;
}


bool Platform::isDebian()
{
    if (isLinux())
    {
        QFileInfo infoRelease("/etc/debian_release");
        QFileInfo infoVersion("/etc/debian_version");
        return infoRelease.exists() || infoVersion.exists();
    }

    return false;
}


bool Platform::isFedora()
{
    if (isLinux())
    {
        QFileInfo info("/etc/fedora-release");
        return info.exists();
    }

    return false;
}


Platform::WindowsVersion Platform::windowsVersion()
{
#if defined(Q_OS_WIN)
    switch (QSysInfo::windowsVersion())
    {
    case QSysInfo::WV_WINDOWS7:
        return Windows7;
        
    case QSysInfo::WV_VISTA:
        return WindowsVista;
        
    case QSysInfo::WV_XP:
        return WindowsXP;
        
    default:
        break;
    }
#endif

    return UnknownWindowsVersion;
}


Platform::MacVersion Platform::macVersion()
{
#if defined(Q_OS_MAC)
    switch (QSysInfo::MacintoshVersion)
    {
    case QSysInfo::MV_10_7:
        return Mac10_7;
        
    case QSysInfo::MV_10_6:
        return Mac10_6;
        
    case QSysInfo::MV_10_5:
        return Mac10_5;
        
    default:
        break;
    }
#endif

    return UnknownMacVersion;
}


Platform::Architecture Platform::architecture()
{
    switch (sizeof(ptrdiff_t))
    {
    case 4:
        return Platform::x86;

    case 8:
        return Platform::x64;

    default:
        return Platform::UnknownArchitecture;
    }
}


QString Platform::os()
{
    QString result;

    if (isWindows())
        result += "windows";
    else if (isMac())
        result += "mac";
    else if (isUbuntu())
        result += "ubuntu";
    else if (isDebian())
        result += "debian";
    else if (isFedora())
        result += "fedora";
    else
        result += "unknown";

    return result;
}

QString Platform::platform()
{
    QString result = os();

    switch (Platform::architecture())
    {
    case Platform::x86:
        result += "-32";
        break;

    case Platform::x64:
        result += "-64";
        break;

    default:
        result += "-unknown";
        break;
    }

    return result;
}

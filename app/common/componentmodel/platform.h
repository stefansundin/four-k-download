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


#ifndef PLATFORM_H
#define PLATFORM_H

#include <QtGlobal>

namespace ComponentModel
{

class Platform
{
public:
    enum Architecture { UnknownArchitecture, x86, x64 };

    static bool isWindows();
    static bool isMac();
    static bool isLinux();
    static bool isUbuntu();
    static bool isDebian();
    static bool isFedora();

    static Architecture architecture();
    static QString os();
    static QString platform();
};

} // Plarform

#endif // PLATFORM_H

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


#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <QString>

namespace ComponentModel
{

class FileSystem
{
public:
    static QString generateFileName(const QString& ext = "");
    static QString generateFilePath(const QString& directory, const QString& filename, const QString& ext = "");
    static QString generateUniqueFilePath(const QString& directory, const QString& filename, const QString& ext = "");
    static QString filterInvalidSymbols(const QString& filename);
};

} // ComponentModel

#endif // FILESYSTEM_H

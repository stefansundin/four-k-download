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


#ifndef COMPONENTMODEL_FILESYSTEM_H
#define COMPONENTMODEL_FILESYSTEM_H

#include <QObject>
#include <QString>

namespace ComponentModel
{

class FileSystem : public QObject
{
    Q_OBJECT

public:
    Q_INVOKABLE static QString changeFilePathExt(const QString& filepath, const QString& ext = "");
    Q_INVOKABLE static QString generateFileName(const QString& ext = "");
    Q_INVOKABLE static QString generateFilePath(const QString& directory, const QString& filename, const QString& ext = "");
    Q_INVOKABLE static QString generateUniqueFilePath(const QString& directory, const QString& filename, const QString& ext = "");
    Q_INVOKABLE static QString filterInvalidSymbols(const QString& filename);

    Q_INVOKABLE static void showFile(const QString& filename);
    Q_INVOKABLE static void openFile(const QString& filename);
};

} // ComponentModel

#endif // COMPONENTMODEL_FILESYSTEM_H

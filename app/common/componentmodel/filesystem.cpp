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


#include "componentmodel/filesystem.h"
#include <QUuid>
#include <QDir>
#include <QDesktopServices>
#include <QUrl>

#if defined(Q_OS_WIN)
#include <windows.h>
#include <Shellapi.h>
#endif 

using namespace ComponentModel;


QString FileSystem::changeFilePathExt(const QString& filepath, const QString& ext)
{
    QFileInfo info(filepath);
    return info.dir().absoluteFilePath(info.completeBaseName() + ext);
}


QString FileSystem::generateFileName(const QString& ext)
{
    QString filename = QUuid::createUuid().toString();
    filename = filename.remove("{", Qt::CaseInsensitive);
    filename = filename.remove("}", Qt::CaseInsensitive);
    return filename + ext;
}


QString FileSystem::generateFilePath(const QString& directory, const QString& filename, const QString& ext)
{
    QDir dir(directory);
    QString filteredFileName = filterInvalidSymbols(filename);
    return dir.absoluteFilePath(filteredFileName + ext);
}


QString FileSystem::generateUniqueFilePath(const QString& directory, const QString& filename, const QString& ext)
{
    QDir dir(directory);
    QString filteredFileName = filterInvalidSymbols(filename);
    QString result = dir.absoluteFilePath(filteredFileName + ext);

    QFileInfo file(result);
    if (!file.exists())
        return result;


    for (int i = 2; i < 10000; ++i)
    {
        result = dir.absoluteFilePath(filteredFileName + " (" + QString::number(i) + ")" + ext);

        QFileInfo file(result);
        if (!file.exists())
            return result;
    }

    return result;
}


QString FileSystem::filterInvalidSymbols(const QString& filename)
{
    QString result = filename;

    result = result.replace(":", " ", Qt::CaseInsensitive);
    result = result.replace("*", " ", Qt::CaseInsensitive);
    result = result.replace("/", " ", Qt::CaseInsensitive);
    result = result.replace("\\", " ", Qt::CaseInsensitive);
    result = result.replace("?", "", Qt::CaseInsensitive);
    result = result.replace("|", " ", Qt::CaseInsensitive);
    result = result.replace("\'", " ", Qt::CaseInsensitive);
    result = result.replace("\"", " ", Qt::CaseInsensitive);
    result = result.replace("<", " ", Qt::CaseInsensitive);
    result = result.replace(">", " ", Qt::CaseInsensitive);

    return result;
}


void FileSystem::showFile(const QString& filename)
{
    QFileInfo info = QFileInfo(filename);
    if (!info.exists())
        return;

#if defined(Q_OS_WIN)
    QString url = info.canonicalFilePath();
    url.replace("/", "\\");
    url = "/select, \"" + url + "\"";
    ShellExecute(NULL, L"open", L"explorer.exe", (WCHAR*)url.utf16(), NULL, 5);
#else
    QString url = info.canonicalPath();
    QDesktopServices::openUrl(QUrl("file:///"+url));
#endif
}


void FileSystem::openFile(const QString& filename)
{
    QFileInfo info = QFileInfo(filename);
    if (!info.exists())
        return;

    QString url = info.canonicalFilePath();
    QDesktopServices::openUrl(QUrl("file:///"+url));
}

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


#include "social/facebook.h"
#include <QDesktopServices>
#include <QUrl>

using namespace Social;


void Facebook::shareLink(QString url, QString text)
{
    QString a = QString("http://www.facebook.com/sharer.php?");
    a += "u=" + url;
    a += "&t=" + text;

    QUrl u("http://www.facebook.com/sharer.php");
    u.addEncodedQueryItem("u", QUrl::toPercentEncoding(url));
    u.addEncodedQueryItem("t", QUrl::toPercentEncoding(text));

    QDesktopServices::openUrl(u);
}

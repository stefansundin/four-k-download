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


#include "social/twitter.h"
#include <QDesktopServices>
#include <QUrl>

using namespace Social;


void Twitter::shareLink(QString url, QString text, QString referer, QString via)
{
    QUrl u("http://twitter.com/intent/tweet");
    u.addEncodedQueryItem("text", QUrl::toPercentEncoding(text));
    u.addEncodedQueryItem("url", QUrl::toPercentEncoding(url));
    if (!referer.isEmpty())
        u.addEncodedQueryItem("original_referer", QUrl::toPercentEncoding(referer));
    if (!via.isEmpty())
        u.addEncodedQueryItem("via", QUrl::toPercentEncoding(via));

    QDesktopServices::openUrl(u);
}

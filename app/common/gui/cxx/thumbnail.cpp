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


#include "gui/cxx/thumbnail.h"

using namespace Gui;


inline void setAlpha(QImage& image, int x, int y, int alpha)
{
    QColor color = image.pixel(x, y);
    color.setAlpha(alpha);
    image.setPixel(x, y, color.rgba());
}


void Thumbnail::apply(const QPixmap& src, QPixmap& dst)
{
    QImage image = src.toImage().
                   scaled(Thumbnail::Width, Thumbnail::Height, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation).
                   convertToFormat(QImage::Format_ARGB32);
    image = (image.width() == Thumbnail::Width) ?
             image.copy(0, (image.height() - Thumbnail::Height) / 2, Thumbnail::Width, Thumbnail::Height) :
             image.copy((image.width() - Thumbnail::Width) / 2, 0, Thumbnail::Width, Thumbnail::Height);

    int bottom = image.height() - 1;
    int right = image.width() - 1;

    setAlpha(image, 0, 0, 0);
    setAlpha(image, 1, 0, 96);
    setAlpha(image, 2, 0, 192);
    setAlpha(image, 0, 1, 96);
    setAlpha(image, 0, 2, 192);

    setAlpha(image, 0, bottom, 0);
    setAlpha(image, 1, bottom, 96);
    setAlpha(image, 2, bottom, 192);
    setAlpha(image, 0, bottom - 1, 96);
    setAlpha(image, 0, bottom - 2, 192);

    setAlpha(image, right, 0, 0);
    setAlpha(image, right - 1, 0, 96);
    setAlpha(image, right - 2, 0, 192);
    setAlpha(image, right, 1, 96);
    setAlpha(image, right, 2, 192);

    setAlpha(image, right, bottom, 0);
    setAlpha(image, right - 1, bottom, 96);
    setAlpha(image, right - 2, bottom, 192);
    setAlpha(image, right, bottom - 1, 96);
    setAlpha(image, right, bottom - 2, 192);

    dst = QPixmap::fromImage(image);
}

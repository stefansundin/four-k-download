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


#ifndef DOWNLOADITEMDELEGATE_H
#define DOWNLOADITEMDELEGATE_H

#include <QItemDelegate>

namespace View
{

class DownloadItemDelegate : public QItemDelegate
{
    Q_OBJECT
    Q_PROPERTY(QRect statusArea READ statusArea)

public:
    Q_INVOKABLE explicit DownloadItemDelegate(QObject* parent = 0);

    virtual void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    virtual QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;

    QRect statusArea() const;

private:
    mutable QRect m_statusArea;
};

} // View

#endif // DOWNLOADITEMDELEGATE_H

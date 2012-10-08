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


/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QDECLARATIVELINEARLAYOUT_H
#define QDECLARATIVELINEARLAYOUT_H

#include "qdeclarativelayout.h"


class QDeclarativeLinearLayout : public QDeclarativeLayout
{
    Q_OBJECT
    Q_PROPERTY(qreal spacing READ spacing WRITE setSpacing NOTIFY spacingChanged)

public:
    enum Orientation {
        Vertical,
        Horizontal
    };

    explicit QDeclarativeLinearLayout(Orientation orientation,
                                      QDeclarativeItem *parent = 0);
    ~QDeclarativeLinearLayout() {}

    qreal spacing() const;
    void setSpacing(qreal spacing);

    Orientation orientation() const;
    void setOrientation(Orientation orientation);

    void componentComplete();

signals:
    void spacingChanged();
    void orientationChanged();

protected:
    void updateLayoutItems();
    void reconfigureLayout();
    void insertLayoutItem(QDeclarativeItem *item);
    void removeLayoutItem(QDeclarativeItem *item);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry);

protected slots:
    void onItemDestroyed();

private:
    qreal m_spacing;
    Orientation m_orientation;
    QList<QDeclarativeItem *> m_items;
};


class QDeclarativeRowLayout : public QDeclarativeLinearLayout
{
    Q_OBJECT

public:
    explicit QDeclarativeRowLayout(QDeclarativeItem *parent = 0)
        : QDeclarativeLinearLayout(Horizontal, parent) {}
};


class QDeclarativeColumnLayout : public QDeclarativeLinearLayout
{
    Q_OBJECT

public:
    explicit QDeclarativeColumnLayout(QDeclarativeItem *parent = 0)
        : QDeclarativeLinearLayout(Vertical, parent) {}
};

#endif

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


#ifndef QTSPLITTERBASE_H
#define QTSPLITTERBASE_H

#include <QtDeclarative>


class QtSplitterAttached : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal minimumWidth READ minimumWidth WRITE setMinimumWidth NOTIFY minimumWidthChanged)
    Q_PROPERTY(qreal maximumWidth READ maximumWidth WRITE setMaximumWidth NOTIFY maximumWidthChanged)
    Q_PROPERTY(qreal minimumHeight READ minimumHeight WRITE setMinimumHeight NOTIFY minimumHeightChanged)
    Q_PROPERTY(qreal maximumHeight READ maximumHeight WRITE setMaximumHeight NOTIFY maximumHeightChanged)
    Q_PROPERTY(qreal percentageSize READ percentageSize WRITE setPercentageSize NOTIFY percentageWidthSize)
    Q_PROPERTY(bool expanding READ expanding WRITE setExpanding NOTIFY expandingChanged)
    Q_PROPERTY(int itemIndex READ itemIndex WRITE setItemIndex NOTIFY itemIndexChanged)

public:
    explicit QtSplitterAttached(QObject *object);

    qreal minimumWidth() const { return m_minimumWidth; }
    void setMinimumWidth(qreal width);

    qreal maximumWidth() const { return m_maximumWidth; }
    void setMaximumWidth(qreal width);

    qreal minimumHeight() const { return m_minimumHeight; }
    void setMinimumHeight(qreal width);

    qreal maximumHeight() const { return m_maximumHeight; }
    void setMaximumHeight(qreal width);

    bool expanding() const { return m_expanding; }
    void setExpanding(bool expanding);

    qreal percentageSize() const { return m_percentageSize; }

    int itemIndex() const { return m_itemIndex; }

    void setPercentageSize(qreal arg) { m_percentageSize = arg; }
    void setItemIndex(int arg) {
        if (m_itemIndex != arg) {
            m_itemIndex = arg;
            emit itemIndexChanged(arg);
        }
    }

signals:
    void minimumWidthChanged(qreal arg);
    void maximumWidthChanged(qreal arg);
    void minimumHeightChanged(qreal arg);
    void maximumHeightChanged(qreal arg);
    void expandingChanged(bool arg);
    void percentageWidthSize(qreal arg);
    void itemIndexChanged(int arg);

private:
    qreal m_minimumWidth;
    qreal m_maximumWidth;
    qreal m_minimumHeight;
    qreal m_maximumHeight;
    qreal m_percentageSize;
    int m_itemIndex;
    bool m_expanding;

    friend class QtSplitterBase;
};


class QtSplitterBase : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit QtSplitterBase(QDeclarativeItem *parent = 0);
    ~QtSplitterBase() {}

    static QtSplitterAttached *qmlAttachedProperties(QObject *object);
};

QML_DECLARE_TYPEINFO(QtSplitterBase, QML_HAS_ATTACHED_PROPERTIES)

#endif // QTSPLITTERBASE_H

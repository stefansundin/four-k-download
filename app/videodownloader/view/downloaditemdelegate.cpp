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


#include "view/downloaditemdelegate.h"
#include "view/downloadlistmodel.h"
#include "viewmodel/downloaditemviewmodel.h"
#include "gui/cxx/thumbnail.h"
#include <QPainter>
#include <QTime>
#include <QApplication>

using namespace View;
using namespace ViewModel;
using namespace Gui;


namespace
{

const int BorderOffset      = 6;
const int ElementOffset     = 7;
const int TextOffset        = 3;

} // Anonimous


DownloadItemDelegate::DownloadItemDelegate(QObject* parent) :
    QItemDelegate(parent)
{
}


void DownloadItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QString title = index.model()->data(index, DownloadListModel::TitleRole).toString();
    QString duration = index.model()->data(index, DownloadListModel::DurationRole).toString();
    QString size = index.model()->data(index, DownloadListModel::SizeRole).toString();
    int progress = index.model()->data(index, DownloadListModel::ProgressRole).toInt();
    QString remainingTime = index.model()->data(index, DownloadListModel::RemainingTimeRole).toString();
    DownloadItemViewModel::State state = index.model()->data(index, DownloadListModel::StateRole).value<DownloadItemViewModel::State>();
    QVariant v = index.model()->data(index, Qt::DecorationRole);
    QPixmap pixmap;
    if (v.canConvert<QPixmap>())
        pixmap = v.value<QPixmap>();

    QStyleOptionViewItem curOption = option;
    curOption.textElideMode = Qt::ElideRight;
    curOption.displayAlignment = Qt::AlignLeft | Qt::AlignTop;


    QRect clip = QRect(curOption.rect.left() + BorderOffset, curOption.rect.top() + BorderOffset,
                       curOption.rect.width() - BorderOffset, curOption.rect.height() - BorderOffset);

    if (parent() && parent()->metaObject()->indexOfProperty("controlAreaIndex") != -1 &&
            parent()->metaObject()->indexOfProperty("controlAreaWidth") != -1)
    {
        if (index.row() == parent()->property("controlAreaIndex").toInt())
            clip.setRight(clip.right() - parent()->property("controlAreaWidth").toInt());
    }

    // Background
    drawBackground(painter, curOption, index);

    // Thumbnail
    painter->drawPixmap(clip.left() + (Thumbnail::Width - pixmap.width()) / 2,
                        clip.top() + (Thumbnail::Height - pixmap.height()) / 2,
                        pixmap);
    clip.setLeft(clip.left() + Thumbnail::Width + BorderOffset);

    // Title area
    QRect textClip = clip;

    curOption.font.setBold(true);
    curOption.fontMetrics = QFontMetrics(curOption.font);

    textClip.setTop(textClip.top() + TextOffset);
    drawDisplay(painter, curOption, textClip, title);
    textClip.setTop(textClip.top() + curOption.fontMetrics.height() + ElementOffset);

    curOption.font.setBold(false);
    curOption.fontMetrics = QFontMetrics(curOption.font);

    // Status area
    curOption.palette.setColor(QPalette::Text, curOption.palette.background().color().darker());

    if (state != DownloadItemViewModel::Parse)
    {
        if (!duration.isEmpty())
        {
            QPixmap itemclock(":/image/item-time");
            painter->drawPixmap(textClip.left(), textClip.top() + (curOption.fontMetrics.height() - itemclock.height() - 1) / 2,
                                itemclock);
            textClip.setLeft(textClip.left() + itemclock.width() + TextOffset);
            drawDisplay(painter, curOption, textClip, duration);
            textClip.setLeft(textClip.left() + curOption.fontMetrics.width(duration) + 2 * ElementOffset);
        }

        QPixmap itemsize(":/image/item-size");
        painter->drawPixmap(textClip.left(), textClip.top() + (curOption.fontMetrics.height() - itemsize.height() - 1) / 2,
                            itemsize);
        textClip.setLeft(textClip.left() + itemsize.width() + TextOffset);
        drawDisplay(painter, curOption, textClip, size);
        textClip.setLeft(textClip.left() + curOption.fontMetrics.width(size) + 2 * ElementOffset);
    }

    switch (state)
    {
    case DownloadItemViewModel::Download:
    case DownloadItemViewModel::Convert:
    case DownloadItemViewModel::Pause:
        {
            QPixmap icon;

            if (state == DownloadItemViewModel::Download)
                icon.load(":/image/item-download-progress");
            else if (state == DownloadItemViewModel::Convert)
                icon.load(":/image/item-convert");
            if (state == DownloadItemViewModel::Pause)
                icon.load(":/image/item-download-paused");

            painter->drawPixmap(textClip.left(), textClip.top() + (curOption.fontMetrics.height() - icon.height() - 1) / 2, icon);
            textClip.setLeft(textClip.left() + icon.width() + TextOffset);

            int progressWidth = qMin(150, textClip.width());
            QStyleOptionProgressBarV2 bar;
            bar.minimum = 0;
            bar.maximum = 100;
            bar.progress = progress;
            bar.rect = QRect(textClip.left(), textClip.top(), progressWidth, curOption.fontMetrics.height());
            QApplication::style()->drawControl( QStyle::CE_ProgressBar, &bar, painter, 0 );
            textClip.setLeft(textClip.left() + progressWidth + TextOffset);

            drawDisplay(painter, curOption, textClip, remainingTime);
        }
        break;

    case DownloadItemViewModel::Done:
        {
            DownloadItemViewModel::Media media = index.model()->data(index, DownloadListModel::MediaRole).value<DownloadItemViewModel::Media>();
            QString text = index.model()->data(index, DownloadListModel::FormatRole).toString();
            QPixmap icon(media == DownloadItemViewModel::Video ? ":/image/item-video" : ":/image/item-audio");
            painter->drawPixmap(textClip.left(), textClip.top() + (curOption.fontMetrics.height() - icon.height() - 1) / 2, icon);
            textClip.setLeft(textClip.left() + icon.width() + TextOffset);
            drawDisplay(painter, curOption, textClip, text);
            textClip.setLeft(textClip.left() + curOption.fontMetrics.width(text) + ElementOffset);
        }
        break;

    case DownloadItemViewModel::Error:
        {
            QString text = tr("Error");
            QPixmap icon(":/image/item-error");
            painter->drawPixmap(textClip.left(), textClip.top() + (curOption.fontMetrics.height() - icon.height() - 1) / 2, icon);
            textClip.setLeft(textClip.left() + icon.width() + TextOffset);
            drawDisplay(painter, curOption, textClip, text);
            textClip.setLeft(textClip.left() + curOption.fontMetrics.width(text) + ElementOffset);
        }
        break;

    default:
        break;
    }

    // Focus
    drawFocus(painter, curOption, curOption.rect);

    // Status area
    textClip.moveLeft(textClip.left() - option.rect.left());
    textClip.moveTop(textClip.top() - option.rect.top());
    m_statusArea = textClip;
}


QSize DownloadItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QSize result = QItemDelegate::sizeHint(option, index);
    result.setHeight(Thumbnail::Height + 2 * BorderOffset);
    return result;
}


QRect DownloadItemDelegate::statusArea() const
{
    return m_statusArea;
}

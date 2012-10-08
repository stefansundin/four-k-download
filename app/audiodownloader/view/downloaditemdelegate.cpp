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
    QString playingTime = index.model()->data(index, DownloadListModel::PlayingTimeRole).toString();
    DownloadItemViewModel::State state = index.model()->data(index, DownloadListModel::StateRole).value<DownloadItemViewModel::State>();
    QPixmap pixmap;
    switch (state)
    {
    case DownloadItemViewModel::DoneState:
    case DownloadItemViewModel::PlayerPauseState:
        pixmap = QPixmap(":/image/item-player-play");
        break;

    case DownloadItemViewModel::PlayerPlayState:
        pixmap = QPixmap(":/image/item-player-pause");
        break;

    default:
        pixmap = QPixmap(":/image/item-audio");
        break;
    }

    QStyleOptionViewItem curOption = option;
    curOption.textElideMode = Qt::ElideRight;
    curOption.displayAlignment = Qt::AlignLeft | Qt::AlignTop;


    QRect clip = QRect(curOption.rect.left() + BorderOffset, curOption.rect.top() + BorderOffset,
                       curOption.rect.width() - BorderOffset, curOption.rect.height() - BorderOffset);
    int controlAreaIndex = (parent() && parent()->metaObject()->indexOfProperty("controlAreaIndex") != -1) ?
                            parent()->property("controlAreaIndex").toInt() :
                            -1;
    int controlAreaWidth = 90;
    int progressWidth = 120;

    QRect textClip = clip;
    if (state == DownloadItemViewModel::DownloadState || state == DownloadItemViewModel::ConvertState ||
        state == DownloadItemViewModel::PauseState || state == DownloadItemViewModel::ErrorState)
        textClip.setRight(clip.right() - controlAreaWidth - progressWidth);
    else if (index.row() == controlAreaIndex)
        textClip.setRight(clip.right() - controlAreaWidth);
    QRect progressClip = QRect(clip.right() - controlAreaWidth - progressWidth, clip.top(), progressWidth, clip.height());
    QRect controlClip = QRect(clip.right() - controlAreaWidth, clip.top(), controlAreaWidth - BorderOffset, clip.height());


    // Background
    drawBackground(painter, curOption, index);

    // Thumbnail
    if (index.row() != controlAreaIndex || (state != DownloadItemViewModel::DoneState &&
                                            state != DownloadItemViewModel::PlayerPlayState &&
                                            state != DownloadItemViewModel::PlayerPauseState))
    {
#if defined(Q_OS_LINUX)
        painter->drawPixmap(textClip.left(), textClip.top(), pixmap);
#else
        painter->drawPixmap(textClip.left(), textClip.top() - 1, pixmap);
#endif
    }

    textClip.setLeft(textClip.left() + pixmap.width() + TextOffset);

    // Title area
    curOption.fontMetrics = QFontMetrics(curOption.font);
    drawDisplay(painter, curOption, textClip, title);
    curOption.fontMetrics = QFontMetrics(curOption.font);

    // Status and control area
    curOption.palette.setColor(QPalette::Text, curOption.palette.background().color().darker());

    switch (state)
    {
    case DownloadItemViewModel::DownloadState:
    case DownloadItemViewModel::ConvertState:
    case DownloadItemViewModel::PauseState:
        {
            QPixmap icon;

            if (state == DownloadItemViewModel::DownloadState)
                icon.load(":/image/item-download-progress");
            else if (state == DownloadItemViewModel::ConvertState)
                icon.load(":/image/item-convert");
            if (state == DownloadItemViewModel::PauseState)
                icon.load(":/image/item-download-paused");

            painter->drawPixmap(progressClip.left(), progressClip.top() + (curOption.fontMetrics.height() - icon.height() - 1) / 2, icon);
            progressClip.setLeft(progressClip.left() + icon.width() + TextOffset);

            int progressWidth = progressClip.width();
            QStyleOptionProgressBarV2 bar;
            bar.minimum = 0;
            bar.maximum = 100;
            bar.progress = progress;
            bar.rect = QRect(progressClip.left(), progressClip.top(), progressWidth, curOption.fontMetrics.height());
            QApplication::style()->drawControl( QStyle::CE_ProgressBar, &bar, painter, 0 );

            if (index.row() != controlAreaIndex)
            {
                curOption.displayAlignment = Qt::AlignRight | Qt::AlignTop;
                drawDisplay(painter, curOption, controlClip, remainingTime);
            }
        }
        break;

    case DownloadItemViewModel::DoneState:
    case DownloadItemViewModel::PlayerPlayState:
    case DownloadItemViewModel::PlayerPauseState:
        {
            if (index.row() != controlAreaIndex)
            {
                curOption.displayAlignment = Qt::AlignRight | Qt::AlignTop;
                if (state == DownloadItemViewModel::PlayerPlayState)
                    drawDisplay(painter, curOption, controlClip, playingTime);
                else
                    drawDisplay(painter, curOption, controlClip, duration);
            }
        }
        break;

    case DownloadItemViewModel::ErrorState:
        {
            QString text = tr("Error");
            QPixmap icon(":/image/item-error");
            painter->drawPixmap(progressClip.left(), progressClip.top() + (curOption.fontMetrics.height() - icon.height() - 1) / 2, icon);
            progressClip.setLeft(progressClip.left() + icon.width() + TextOffset);
            drawDisplay(painter, curOption, progressClip, text);
        }
        break;

    default:
        break;
    }

    // Focus
    drawFocus(painter, curOption, curOption.rect);
}


QSize DownloadItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItem curOption = option;
    curOption.fontMetrics = QFontMetrics(curOption.font);

    QSize result = QItemDelegate::sizeHint(option, index);
    result.setHeight(curOption.fontMetrics.height() + 2 * BorderOffset);
    return result;
}

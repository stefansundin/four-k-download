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


#include "viewmodel/mediaitemviewmodel.h"
#include "gui/cxx/thumbnail.h"
#include "componentmodel/transform.h"
#include <QEvent>
#include <QPointer>
#include <QCoreApplication>

using namespace ViewModel;
using namespace ComponentModel;
using namespace Gui;
using namespace openmedia;


namespace
{

const QEvent::Type InitializeResultEventType = QEvent::Type(QEvent::User + 5);


class InitializeResultEvent : public QEvent
{
public:
    InitializeResultEvent(downloader::media_download_list::InitializedResult result) :
            QEvent(InitializeResultEventType),
            m_result(result)
    {
    }

    downloader::media_download_list::InitializedResult result()
    {
        return m_result;
    }

private:
    downloader::media_download_list::InitializedResult m_result;
};


class InitializeResultFunctor
{
public:
    InitializeResultFunctor(QObject* item) :
            m_item(item)
    {
    }

    void operator () (downloader::media_download_list::InitializedResult result)
    {
        if (!m_item.isNull())
            QCoreApplication::postEvent(m_item.data(), new InitializeResultEvent(result));
    }

private:
    QPointer<QObject> m_item;
};

} // Anonimous


MediaItemViewModel::MediaItemViewModel(const Mvvm::Dialog* dialog, openmedia::downloader::media_download_list_ptr item, QObject *parent) :
    NotifyObject(parent),
    m_dialog(dialog),
    m_item(item),
    m_checked(true),
    m_state(Parsing)
{
    qRegisterMetaType<MediaItemViewModel::State>();

    m_item->initialize(InitializeResultFunctor(this));
}


QPixmap MediaItemViewModel::thumbnail() const
{
    QPixmap result;

    try
    {
        Thumbnail::apply(QPixmap(":/image/thumbnail-default"), result);

        if (m_item->thumbnail().get())
        {
            QImage image = QImage::fromData(&(m_item->thumbnail()->at(0)), m_item->thumbnail()->size(), "jpg");
            Thumbnail::apply(QPixmap::fromImage(image), result);
        }
    }
    catch(...)
    {
    }

    return result;
}


QString MediaItemViewModel::title() const
{
    QString title = QString::fromStdWString(m_item->title());
#if defined(Q_OS_WIN)
    if (title.length() > 50)
        title = title.left(50) + "...";
#else
    if (title.length() > 45)
        title = title.left(45) + "...";
#endif

    return title;
}


QString MediaItemViewModel::duration() const
{
    if (m_item->duration() == 0)
        return QString();

    return Transform::timeToString(m_item->duration(), "H:mm:ss");
}


QString MediaItemViewModel::url() const
{
    QString url = QString::fromStdString(m_item->url());
#if defined(Q_OS_WIN)
    if (url.length() > 50)
        url = url.left(50) + "...";
#else
    if (url.length() > 40)
        url = url.left(40) + "...";
#endif

    return QString("<a href=\"") + QString::fromStdString(m_item->url()) + "\">" + url + "</a>";
}


bool MediaItemViewModel::isChecked() const
{
    return m_checked;
}


void MediaItemViewModel::setChecked(bool value)
{
    if (state() == Error)
        value = false;

    if (m_checked != value)
    {
        m_checked = value;
        emitPropertyChanged("isChecked", m_checked);
    }
}


MediaItemViewModel::State MediaItemViewModel::state() const
{
    return m_state;
}


bool MediaItemViewModel::event(QEvent* e)
{
    switch (e->type())
    {
    case InitializeResultEventType:
        {
            InitializeResultEvent* ie = (InitializeResultEvent*)e;

            if (ie->result() == downloader::media_download_list::initFailed)
            {
                m_state = Error;
                setChecked(false);
            }
            else
                m_state = Done;

            emitPropertyChanged("thumbnail");
            emitPropertyChanged("title");
            emitPropertyChanged("duration");
            emitPropertyChanged("url");
            emitPropertyChanged("state", QVariant::fromValue(m_state));
        }
        return true;

    default:
        return false;
    }

}

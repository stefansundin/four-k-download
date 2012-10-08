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


#ifndef MEDIAITEMVIEWMODEL_H
#define MEDIAITEMVIEWMODEL_H

#include <QObject>
#include "mvvm/cxx/dialog.h"
#include "componentmodel/notifyobject.h"
#include "openmedia/DTMediaDownloader.h"


namespace ViewModel
{

class MediaItemViewModel : public ComponentModel::NotifyObject
{
    Q_OBJECT
    Q_PROPERTY(QPixmap thumbnail READ thumbnail)
    Q_PROPERTY(QString title READ title)
    Q_PROPERTY(QString duration READ duration)
    Q_PROPERTY(QString url READ url)
    Q_PROPERTY(bool isChecked READ isChecked WRITE setChecked)
    Q_PROPERTY(State state READ state)
    Q_ENUMS(State)

public:
    enum State { Parsing, Done, Error };

    MediaItemViewModel(const Mvvm::Dialog* dialog, openmedia::downloader::media_download_list_ptr item, QObject *parent = 0);

    QPixmap thumbnail() const;
    QString title() const;
    QString duration() const;
    QString url() const;
    bool isChecked() const;
    void setChecked(bool value);
    State state() const;

protected:
    virtual bool event(QEvent* e);

private:
    QWeakPointer<const Mvvm::Dialog> m_dialog;
    openmedia::downloader::media_download_list_ptr m_item;    
    bool m_checked;
    State m_state;
};

} // ViewModel

Q_DECLARE_METATYPE(ViewModel::MediaItemViewModel::State)

#endif // MEDIAITEMVIEWMODEL_H

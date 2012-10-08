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


#ifndef SMARTMODEVIEWMODEL_H
#define SMARTMODEVIEWMODEL_H

#include <QObject>
#include <QAction>
#include "mvvm/cxx/dialog.h"
#include "componentmodel/notifyobject.h"
#include "viewmodel/mediaitemviewmodel.h"
#include "viewmodel/mediadetailsviewmodel.h"
#include "openmedia/DTMediaDownloader.h"

namespace ViewModel
{

class SmartModeViewModel : public ComponentModel::NotifyObject
{
    Q_OBJECT
    Q_PROPERTY(bool smartMode READ isSmartMode WRITE setSmartMode)
    Q_PROPERTY(QString dirName READ dirName WRITE setDirName)
    Q_PROPERTY(QStringList qualityList READ qualityList)
    Q_PROPERTY(int qualityIndex READ qualityIndex WRITE setQualityIndex)
    Q_PROPERTY(bool qualityEnabled READ qualityEnabled)
    Q_PROPERTY(QStringList formatList READ formatList)
    Q_PROPERTY(int formatIndex READ formatIndex WRITE setFormatIndex)
    Q_PROPERTY(QAction* dirAction READ dirAction)

public:
    SmartModeViewModel(const Mvvm::Dialog* dialog, QObject *parent = 0);
    virtual ~SmartModeViewModel();

    bool isSmartMode() const;
    void setSmartMode(bool value);
    QString dirName() const;
    void setDirName(QString value);
    QStringList qualityList() const;
    int qualityIndex() const;
    void setQualityIndex(int value);
    bool qualityEnabled() const;
    QStringList formatList() const;
    int formatIndex() const;
    void setFormatIndex(int value);
    QAction* dirAction();

public slots:
    void saveState();

protected slots:
    void browse();

protected:
    void initialize();

private:
    QWeakPointer<const Mvvm::Dialog> m_dialog;
    bool m_smartMode;
    QString m_dirName;
    int m_qualityIndex;
    int m_formatIndex;
    QAction m_dirAction;
};

} // ViewModel

#endif // SMARTMODEVIEWMODEL_H

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


#include "viewmodel/downloadlistviewmodel.h"
#include <stdexcept>

using namespace ViewModel;
using namespace ComponentModel;


DownloadListViewModel::DownloadListViewModel(QObject *parent) :
    NotifyList(parent)
{
}


void DownloadListViewModel::checkObject(const QSharedPointer<QObject>& object)
{
    if (!object->inherits("ViewModel::DownloadItemViewModel"))
        throw std::invalid_argument("Object must inherit ViewModel::DownloadItemViewModel.");
}

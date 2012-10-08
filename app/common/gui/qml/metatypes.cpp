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


#include "gui/qml/metatypes.h"
#include "gui/qml/droparea.h"
#include "gui/qml/cursorshapearea.h"
#include "gui/qml/desktop/src/qstyleitem.h"
#include "gui/qml/desktop/src/qrangemodel.h"
#include "gui/qml/desktop/src/qtmenu.h"
#include "gui/qml/desktop/src/qtmenubar.h"
#include "gui/qml/desktop/src/qwindowitem.h"
#include "gui/qml/desktop/src/qdesktopitem.h"
#include "gui/qml/desktop/src/qwheelarea.h"
#include "gui/qml/desktop/src/qcursorarea.h"
#include "gui/qml/desktop/src/qtooltiparea.h"
#include "gui/qml/desktop/src/qtsplitterbase.h"
#include "gui/qml/desktop/src/qdeclarativelinearlayout.h"
#include "gui/qml/desktop/src/settings.h" 
#include <QDeclarativeItem>

using namespace Gui::Qml;


int MetaTypeInit::initCount = 0;


MetaTypeInit::MetaTypeInit()
{
    if (initCount++ == 0)
    {
        qmlRegisterType<Gui::DropArea>("Gui", 1, 0, "DropArea");
        qmlRegisterType<Gui::CursorShapeArea>("Gui", 1, 0, "CursorShapeArea");

        qmlRegisterType<QStyleItem>("Desktop", 0, 1, "StyleItem");
        qmlRegisterType<QCursorArea>("Desktop", 0, 1, "CursorArea");
        qmlRegisterType<QTooltipArea>("Desktop", 0, 1, "TooltipArea");
        qmlRegisterType<QRangeModel>("Desktop", 0, 1, "RangeModel");
        qmlRegisterType<QWheelArea>("Desktop", 0, 1, "WheelArea");

        qmlRegisterType<QtMenu>("Desktop", 0, 1, "Menu");
        qmlRegisterType<QtMenuBar>("Desktop", 0, 1, "MenuBar");
        qmlRegisterType<QtMenuItem>("Desktop", 0, 1, "MenuItem");
        qmlRegisterType<QtMenuSeparator>("Desktop", 0, 1, "Separator");

        qmlRegisterType<QFileSystemModel>("Desktop", 0, 1, "FileSystemModel");
        qmlRegisterType<QtSplitterBase>("Desktop", 0, 1, "Splitter");
        qmlRegisterType<Settings>("Desktop", 0, 1, "Settings");
        qmlRegisterType<QWindowItem>("Desktop", 0, 1, "Window");

        qmlRegisterUncreatableType<QtMenuBase>("Desktop", 0, 1, "NativeMenuBase", QLatin1String("Do not create objects of type NativeMenuBase"));
        qmlRegisterType<QDeclarativeRowLayout>("Desktop", 0, 1, "RowLayout");
        qmlRegisterType<QDeclarativeColumnLayout>("Desktop", 0, 1, "ColumnLayout");
        qmlRegisterUncreatableType<QDeclarativeLayout>("Desktop", 0, 1, "Layout",
                                                       QLatin1String("Do not create objects of type Layout"));
        qmlRegisterUncreatableType<QDesktopItem>("Desktop", 0, 1, "Desktop", QLatin1String("Do not create objects of type Desktop")); 
    }
}


MetaTypeInit::~MetaTypeInit()
{
    if (--initCount == 0)
    {
    }
}

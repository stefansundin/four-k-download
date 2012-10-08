TARGET = appguiqml
TEMPLATE = lib
CONFIG += staticlib

QT += core declarative script


include ($$SOURCE_TREE/common.pri)


INCLUDEPATH += \
    $$PWD

HEADERS += \
    gui/qml/droparea.h \
    gui/qml/cursorshapearea.h \
    gui/qml/metatypes.h \
    gui/qml/desktop/src/qcursorarea.h \
    gui/qml/desktop/src/qdeclarativefolderlistmodel.h \
    gui/qml/desktop/src/qdeclarativelayout.h \
    gui/qml/desktop/src/qdeclarativelayoutengine_p.h \
    gui/qml/desktop/src/qdeclarativelinearlayout.h \
    gui/qml/desktop/src/qdesktopitem.h \
    gui/qml/desktop/src/qrangemodel.h \
    gui/qml/desktop/src/qrangemodel_p.h \
    gui/qml/desktop/src/qstyleitem.h \
    gui/qml/desktop/src/qtmenu.h \
    gui/qml/desktop/src/qtmenubar.h \
    gui/qml/desktop/src/qtmenuitem.h \
    gui/qml/desktop/src/qtooltiparea.h \
    gui/qml/desktop/src/qtoplevelwindow.h \
    gui/qml/desktop/src/qtsplitterbase.h \
    gui/qml/desktop/src/qwheelarea.h \
    gui/qml/desktop/src/qwindowitem.h \
    gui/qml/desktop/src/settings.h

SOURCES += \
    gui/qml/droparea.cpp \
    gui/qml/cursorshapearea.cpp \
    gui/qml/metatypes.cpp \
    gui/qml/desktop/src/qcursorarea.cpp \
    gui/qml/desktop/src/qdeclarativefolderlistmodel.cpp \
    gui/qml/desktop/src/qdeclarativelayout.cpp \
    gui/qml/desktop/src/qdeclarativelayoutengine.cpp \
    gui/qml/desktop/src/qdeclarativelinearlayout.cpp \
    gui/qml/desktop/src/qdesktopitem.cpp \
    gui/qml/desktop/src/qrangemodel.cpp \
    gui/qml/desktop/src/qstyleitem.cpp \
    gui/qml/desktop/src/qtmenu.cpp \
    gui/qml/desktop/src/qtmenubar.cpp \
    gui/qml/desktop/src/qtmenuitem.cpp \
    gui/qml/desktop/src/qtooltiparea.cpp \
    gui/qml/desktop/src/qtoplevelwindow.cpp \
    gui/qml/desktop/src/qtsplitterbase.cpp \
    gui/qml/desktop/src/qwheelarea.cpp \
    gui/qml/desktop/src/qwindowitem.cpp \
    gui/qml/desktop/src/settings.cpp

RESOURCES += \
    resource/guiqml.qrc

DEPENDENCY_LIBS = \
    appcomponentmodel
    
include ($$SOURCE_TREE/dependencies.pri)

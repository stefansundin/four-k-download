TEMPLATE = lib
TARGET = appcommon

include ($$SOURCE_TREE/common.pri)

QT += core gui

CONFIG += staticlib

DEFINES += URDL_DISABLE_SSL
win32 {
    DEFINES += _WIN32_WINNT=0x0501
    DEFINES += WIN32_LEAN_AND_MEAN=1
}

INCLUDEPATH += \
    $$SOURCE_TREE/sdk/include \
    $$SOURCE_TREE/3rdparty/urdl/include \
    $$PWD

HEADERS += \ 
    componentmodel/binding/converter.h \
    componentmodel/binding/binding.h \
    componentmodel/transform.h \
    componentmodel/singleapplication.h \
    componentmodel/signalblocker.h \
    componentmodel/signalargs.h \
    componentmodel/selectionchangedsignalargs.h \
    componentmodel/propertychangedsignalargs.h \
    componentmodel/progress.h \
    componentmodel/platform.h \
    componentmodel/notifyselection.h \
    componentmodel/notifyobject.h \
    componentmodel/notifylist.h \
    componentmodel/metatypes.h \
    componentmodel/metaobjectmap.h \
    componentmodel/listchangedsignalargs.h \
    gui/animation/manualtransition.h \
    gui/binding/timetostringconverter.h \
    gui/binding/stringformatconverter.h \
    gui/binding/labelactionbinding.h \
    gui/binding/counttoboolconverter.h \
    gui/binding/comboboxbinding.h \
    gui/binding/buttonactionbinding.h \
    gui/component/volumeslider.h \
    gui/component/limitedslider.h \
    gui/component/hoverlistview.h \
    gui/component/flowlayout.h \
    gui/component/extendedbutton.h \
    gui/component/draggablebutton.h \
    gui/thumbnail.h \
    mvvm/factory.h \
    mvvm/dialog.h \
    componentmodel/filesystem.h \
    gui/binding/elidedtextconverter.h \
    social/facebook.h \
    social/twitter.h
            
SOURCES += \ 
    componentmodel/binding/converter.cpp \
    componentmodel/binding/binding.cpp \
    componentmodel/transform.cpp \
    componentmodel/singleapplication.cpp \
    componentmodel/signalblocker.cpp \
    componentmodel/signalargs.cpp \
    componentmodel/selectionchangedsignalargs.cpp \
    componentmodel/propertychangedsignalargs.cpp \
    componentmodel/progress.cpp \
    componentmodel/platform.cpp \
    componentmodel/notifyselection.cpp \
    componentmodel/notifyobject.cpp \
    componentmodel/notifylist.cpp \
    componentmodel/metatypes.cpp \
    componentmodel/metaobjectmap.cpp \
    componentmodel/listchangedsignalargs.cpp \
    gui/animation/manualtransition.cpp \
    gui/binding/timetostringconverter.cpp \
    gui/binding/stringformatconverter.cpp \
    gui/binding/labelactionbinding.cpp \
    gui/binding/counttoboolconverter.cpp \
    gui/binding/comboboxbinding.cpp \
    gui/binding/buttonactionbinding.cpp \
    gui/component/volumeslider.cpp \
    gui/component/limitedslider.cpp \
    gui/component/hoverlistview.cpp \
    gui/component/flowlayout.cpp \
    gui/component/extendedbutton.cpp \
    gui/component/draggablebutton.cpp \
    gui/thumbnail.cpp \
    mvvm/factory.cpp \
    mvvm/dialog.cpp \
    componentmodel/filesystem.cpp \
    gui/binding/elidedtextconverter.cpp \
    social/facebook.cpp \
    social/twitter.cpp

DEPENDENCY_LIBS = \
    sdkcommon \
    urdl
    
include ($$SOURCE_TREE/dependencies.pri)









TARGET = appguicxx
TEMPLATE = lib
CONFIG += staticlib

QT += core gui declarative


include ($$SOURCE_TREE/common.pri)


INCLUDEPATH += \
    $$PWD

HEADERS += \ 
    gui/cxx/manualtransition.h \
    gui/cxx/timetostringconverter.h \
    gui/cxx/stringformatconverter.h \
    gui/cxx/labelactionbinding.h \
    gui/cxx/counttoboolconverter.h \
    gui/cxx/comboboxbinding.h \
    gui/cxx/buttonactionbinding.h \
    gui/cxx/elidedtextconverter.h \
    gui/cxx/component/volumeslider.h \
    gui/cxx/component/limitedslider.h \
    gui/cxx/component/hoverlistview.h \
    gui/cxx/component/flowlayout.h \
    gui/cxx/component/extendedbutton.h \
    gui/cxx/component/draggablebutton.h \
    gui/cxx/thumbnail.h \
    gui/cxx/metatypes.h
            
SOURCES += \ 
    gui/cxx/manualtransition.cpp \
    gui/cxx/timetostringconverter.cpp \
    gui/cxx/stringformatconverter.cpp \
    gui/cxx/labelactionbinding.cpp \
    gui/cxx/counttoboolconverter.cpp \
    gui/cxx/comboboxbinding.cpp \
    gui/cxx/buttonactionbinding.cpp \
    gui/cxx/elidedtextconverter.cpp \
    gui/cxx/component/volumeslider.cpp \
    gui/cxx/component/limitedslider.cpp \
    gui/cxx/component/hoverlistview.cpp \
    gui/cxx/component/flowlayout.cpp \
    gui/cxx/component/extendedbutton.cpp \
    gui/cxx/component/draggablebutton.cpp \
    gui/cxx/thumbnail.cpp \
    gui/cxx/metatypes.cpp

DEPENDENCY_LIBS = \
    appcomponentmodel
    
include ($$SOURCE_TREE/dependencies.pri)









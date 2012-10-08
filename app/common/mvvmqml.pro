TARGET = appmvvmqml
TEMPLATE = lib
CONFIG += staticlib

QT += core gui declarative


include ($$SOURCE_TREE/common.pri)


INCLUDEPATH += \
    $$PWD

HEADERS += \ 
    mvvm/qml/dialog.h \
    mvvm/qml/metatypes.h
            
SOURCES += \ 
    mvvm/qml/dialog.cpp \
    mvvm/qml/metatypes.cpp

DEPENDENCY_LIBS = \
    appcomponentmodel
    
include ($$SOURCE_TREE/dependencies.pri)









TARGET = appmvvmcxx
TEMPLATE = lib
CONFIG += staticlib

QT += core gui declarative


include ($$SOURCE_TREE/common.pri)


INCLUDEPATH += \
    $$PWD

HEADERS += \ 
    mvvm/cxx/factory.h \
    mvvm/cxx/dialog.h \
    mvvm/cxx/metatypes.h
            
SOURCES += \ 
    mvvm/cxx/factory.cpp \
    mvvm/cxx/dialog.cpp \
    mvvm/cxx/metatypes.cpp

DEPENDENCY_LIBS = \
    appcomponentmodel
    
include ($$SOURCE_TREE/dependencies.pri)









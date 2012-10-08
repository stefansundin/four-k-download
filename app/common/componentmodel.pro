TARGET = appcomponentmodel
TEMPLATE = lib
CONFIG += staticlib

QT += core gui declarative


include ($$SOURCE_TREE/common.pri)


INCLUDEPATH += \
    $$PWD

HEADERS += \ 
    componentmodel/propertyvalueconverter.h \
    componentmodel/propertybinding.h \
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
    componentmodel/notifylistmodel.h \
    componentmodel/metatypes.h \
    componentmodel/metaobjectmap.h \
    componentmodel/listchangedsignalargs.h \
    componentmodel/imageprovider.h \
    componentmodel/proxyimageprovider.h \
    componentmodel/filesystem.h \
    componentmodel/xmlserializable.h \
    componentmodel/objectlessee.h \
    componentmodel/objectlessor.h
            
SOURCES += \ 
    componentmodel/propertyvalueconverter.cpp \
    componentmodel/propertybinding.cpp \
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
    componentmodel/notifylistmodel.cpp \
    componentmodel/metatypes.cpp \
    componentmodel/metaobjectmap.cpp \
    componentmodel/listchangedsignalargs.cpp \
    componentmodel/filesystem.cpp \
    componentmodel/imageprovider.cpp \
    componentmodel/proxyimageprovider.cpp \
    componentmodel/objectlessor.cpp

DEPENDENCY_LIBS =
    
include ($$SOURCE_TREE/dependencies.pri)









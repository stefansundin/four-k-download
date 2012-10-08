TARGET = appmultimedia
TEMPLATE = lib
CONFIG += staticlib

QT += core gui declarative


include ($$SOURCE_TREE/common.pri)


INCLUDEPATH += \
    $$BOOST_PATH/include \
    $$PORTAUDIO_PATH/include \
    $$SOURCE_TREE/3rdparty/portaudio++/include \
    $$SOURCE_TREE/sdk/include \
    $$PWD

HEADERS += \ 
    multimedia/player.h \
    multimedia/audioplayer.h \
    multimedia/metatypes.h
            
SOURCES += \ 
    multimedia/player.cpp \
    multimedia/audioplayer.cpp \
    multimedia/metatypes.cpp

DEPENDENCY_LIBS = \
    sdkmedia \
    sdkcommon \
    portaudioxx
    
include ($$SOURCE_TREE/dependencies.pri)

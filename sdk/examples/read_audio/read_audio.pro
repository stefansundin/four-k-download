TEMPLATE = app
TARGET = read_audio

include ($$SOURCE_TREE/common.pri)

QT += core
QT -= gui

CONFIG += console
CONFIG -= app_bundle

DEFINES += __STDC_CONSTANT_MACROS

INCLUDEPATH += \
    $$SOURCE_TREE/sdk/include

SOURCES += \
    read_audio.cpp

LIBS += \
    -lsdkdownload \
    -lsdkcommon \
    -lurdl

win32 {
LIBS += \
    -lws2_32 \
    -lmswsock
}

LIBS += \
    -lboost_program_options-mt \
    -lboost_system-mt \
    -lboost_filesystem-mt \
    -lboost_date_time-mt \
    -lboost_thread-mt \
    -lboost_regex-mt \
    -lboost_date_time-mt \
    -lboost_regex-mt \
    -lavcodec \
    -lavutil \
    -lavformat \
    -lmp3lame

DEPENDENCY_LIBS = \
    sdlcommon \
    sdkdownload

include ($$SOURCE_TREE/dependencies.pri)

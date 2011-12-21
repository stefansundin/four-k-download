TEMPLATE = app
TARGET = download_youtube

include ($$SOURCE_TREE/common.pri)

QT += core
QT -= gui
QT += script core
QT += scripttools

CONFIG -= app_bundle

INCLUDEPATH += \
    $$SOURCE_TREE/sdk/include

SOURCES += \
    download_youtube.cpp

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
    -lboost_regex-mt

DEPENDENCY_LIBS = \
    sdkcommon \
    sdkdownload

include ($$SOURCE_TREE/dependencies.pri)

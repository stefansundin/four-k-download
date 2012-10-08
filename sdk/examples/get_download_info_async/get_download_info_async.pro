TEMPLATE = app
TARGET = get_download_info_async

include ($$SOURCE_TREE/common.pri)

QT += script core
QT += scripttools
QT -= gui

CONFIG += console
CONFIG -= app_bundle

INCLUDEPATH += \
    $$SOURCE_TREE/sdk/include \
    $$BOOST_PATH/include
    
LIBS += \
    -L$$BOOST_PATH/lib \
    -L$$FFMPEG_PATH/lib \
    -L$$LAME_PATH/lib    
    

SOURCES += \
    get_download_info_async.cpp

LIBS += \
    -lsdkvideodownload \
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
    -lboost_thread-mt \
    -lboost_date_time-mt \
    -lboost_system-mt \
    -lboost_filesystem-mt \
    -lboost_regex-mt \
    -lboost_chrono-mt \


DEPENDENCY_LIBS = \
    sdkvideodownload \
    sdkcommon \
    sdkdownload

include ($$SOURCE_TREE/dependencies.pri)

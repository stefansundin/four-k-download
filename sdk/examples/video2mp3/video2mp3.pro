TEMPLATE = app
TARGET = video2mp3

include ($$SOURCE_TREE/common.pri)

QT += core
QT -= gui

CONFIG += console
CONFIG -= app_bundle

INCLUDEPATH += \
    $$SOURCE_TREE/sdk/include \
    $$BOOST_PATH/include

SOURCES += \
    video2mp3.cpp
    
LIBS += \
    -L$$BOOST_PATH/lib \
    -L$$FFMPEG_PATH/lib \
    -L$$LAME_PATH/lib        

LIBS += \
    -lsdkdownload \
    -lsdkmedia \
    -lsdkcommon

win32 {
LIBS += \
    -lws2_32 \
    -lmswsock \
}

LIBS += \
    -lboost_program_options-mt \
    -lboost_thread-mt \
    -lboost_date_time-mt \
    -lboost_system-mt \
    -lboost_filesystem-mt \
    -lboost_regex-mt \
    -lboost_chrono-mt \
    -lswresample \    
    -lavcodec \
    -lavutil \
    -lavformat \
    -lmp3lame

DEPENDENCY_LIBS = \
    sdkmedia \
    sdkcommon

include ($$SOURCE_TREE/dependencies.pri)

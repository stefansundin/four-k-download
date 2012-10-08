TARGET = 4kvideotomp3
TEMPLATE = app

QT += core gui declarative
QT += xml xmlpatterns
QT += script
QT += scripttools

VERSION = 1.2.0.150
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
DEFINES += __STDC_CONSTANT_MACROS

mac {
    ICON = resource/application/icon.icns
    LIBS += -framework Carbon
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.5
    CONFIG += x86_64
}
win32 {
    RC_FILE = resource/application.rc
}


include ($$SOURCE_TREE/common.pri)


INCLUDEPATH += \
    $$BOOST_PATH/include \
    $$FFMPEG_PATH/include \
    $$LAME_PATH/include \
    $$PORTAUDIO_PATH/include \
    $$SOURCE_TREE/app/common \
    $$SOURCE_TREE/sdk/include \
    $$SOURCE_TREE/3rdparty/portaudio++/include \
    $$SOURCE_TREE/3rdparty/urdl/include \
    $$PWD

SOURCES += \
    main.cpp \
    viewmodel/mainviewmodel.cpp \
    viewmodel/mediaitemviewmodel.cpp

HEADERS  += \
    viewmodel/mainviewmodel.h \
    viewmodel/mediaitemviewmodel.h

TRANSLATIONS = \
    resource/translation/videotomp3_en.ts

RESOURCES += \
    resource/image.qrc \
    resource/qml.qrc
    
OTHER_FILES += \
    view/mainview.qml
    
hack_lupdate {
SOURCES += \
    view/mainview.qml
}
    
LIBS += \
    -L$$BOOST_PATH/lib \
    -L$$FFMPEG_PATH/lib \
    -L$$LAME_PATH/lib \
    -L$$PORTAUDIO_PATH/lib

!mac {
LIBS += \
    -Wl,--start-group
}

LIBS += \
    -lappguiqml \
    -lappmultimedia \
    -lappmvvmqml \
    -lappcomponentmodel \
    -lsdkvideodownload \
    -lsdkdownload \
    -lsdkmedia \
    -lsdkcommon \
    -lportaudioxx \
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
    -lavcodec \
    -lavutil \
    -lavformat \
    -lswresample \    
    -lswscale \
    -lmp3lame \
    -lportaudio

!mac {
LIBS += \
    -Wl,--end-group 
}    

DEPENDENCY_LIBS = \
    appguiqml \
    appmultimedia \
    appmvvmqml \
    appcomponentmodel \
    sdkvideodownload \
    sdkdownload \
    sdkmedia \
    sdkcommon \
    portaudioxx \
    urdl

include ($$SOURCE_TREE/dependencies.pri)

TEMPLATE = app
TARGET = 4kyoutubetomp3

include ($$SOURCE_TREE/common.pri)

QT += core gui
QT += xml xmlpatterns
QT += script
QT += scripttools

DEFINES += __STDC_CONSTANT_MACROS


mac {
    ICON = resource/application/icon.icns
    LIBS += -framework Cocoa
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.5
    CONFIG += x86_64
}
win32 {
    RC_FILE = resource/application.rc
}

VERSION = 1.6.1.100
DEFINES += APP_VERSION=\\\"$$VERSION\\\"


INCLUDEPATH += \
    $$SOURCE_TREE/app/common \
    $$SOURCE_TREE/app/common/gui/component \
    $$SOURCE_TREE/sdk/include \
    $$SOURCE_TREE/3rdparty/urdl/include \
    $$PWD

SOURCES += \
    main.cpp \
    viewmodel/mainviewmodel.cpp \
    view/mainview.cpp \
    viewmodel/downloadlistviewmodel.cpp \
    viewmodel/downloaditemviewmodel.cpp \
    view/selectionmodel.cpp \
    view/downloadlistmodel.cpp \
    view/downloaditemdelegate.cpp

HEADERS  += \
    viewmodel/mainviewmodel.h \
    view/mainview.h \
    viewmodel/downloadlistviewmodel.h \
    viewmodel/downloaditemviewmodel.h \
    view/selectionmodel.h \
    view/downloadlistmodel.h \
    view/downloaditemdelegate.h

FORMS += \
    view/mainview.ui

TRANSLATIONS = \
    resource/translation/audiodownloader_en.ts

RESOURCES += \
    resource/image.qrc
    
LIBS += \
    -lappcommon \
    -lsdkdownload \
    -lsdkmedia \
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
    -lavcodec \
    -lavutil \
    -lavformat \
    -lmp3lame

DEPENDENCY_LIBS = \
    appcommon \
    sdkdownload \
    sdkmedia \
    sdkcommon \
    urdl

include ($$SOURCE_TREE/dependencies.pri)

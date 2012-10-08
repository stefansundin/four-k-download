TARGET = sdkdownload
TEMPLATE = lib
CONFIG += staticlib

QT -= core gui

DEFINES += URDL_DISABLE_SSL
#DEFINES += URDL_HEADER_ONLY
win32 {
    DEFINES += _WIN32_WINNT=0x0501
    DEFINES += WIN32_LEAN_AND_MEAN=1
}


include ($$SOURCE_TREE/common.pri)


INCLUDEPATH += \
    $$BOOST_PATH/include \
    $$SOURCE_TREE/3rdparty/urdl/include \
    $$SOURCE_TREE/sdk/src/ \
    include
    
HEADERS += \    
    include/openmedia/DTConfig.h \
    include/openmedia/DTPlatform.h \
    include/openmedia/DTTypes.h \
    src/DTDownloaderUtils.h \
    include/openmedia/DTHttpDownloader.h \
    src/DTHttpDownloaderImpl.h \
    src/DTIOService.h \
    src/DTHttpDownloaderInstance.h \
    include/openmedia/DTController.h \
    include/openmedia/DTControllerImpl.h \
    include/openmedia/DTHttpDownloader2.h
    
SOURCES += \    
    src/DTHttpDownloader.cpp \
    src/DTHttpDownloader2.cpp \
    src/DTHttpDownloaderImpl.cpp \
    src/DTHttpDownloaderMulti.cpp \
    src/DTHttpDownloaderSimple.cpp \
    src/DTHttpDownloaderSimple2.cpp \
    src/DTController.cpp \
    src/DTControllerImpl.cpp \
    src/DTDownloaderUtils.cpp \
    src/DTIOService.cpp
       
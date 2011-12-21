TEMPLATE = lib
TARGET = sdkdownload

include ($$SOURCE_TREE/common.pri)

QT -= core gui

CONFIG += staticlib
                
DEFINES += URDL_DISABLE_SSL
#DEFINES += URDL_HEADER_ONLY
win32 {
    DEFINES += _WIN32_WINNT=0x0501
    DEFINES += WIN32_LEAN_AND_MEAN=1
}

INCLUDEPATH += \
    include \
    $$SOURCE_TREE/3rdparty/urdl/include
                
HEADERS += \
    src/DTDownloaderUtils.h \
    src/DTDownloaderXnxx.h \
    src/DTDownloaderYoutube.h \
    src/DTDownloaderYoutubeDetails.h \
    src/DTMediaDownloaderDetails.h \
    src/DTSimpleMediaParser.h \
    include/openmedia/DTConfig.h \
    include/openmedia/DTHeaders.h \
    include/openmedia/DTHttpDownloader.h \
    include/openmedia/DTMediaDownloader.h \
    src/DTMediaDownloaderImpl.h \
    include/openmedia/DTMediaSite.h \
    include/openmedia/DTPlatform.h \
    include/openmedia/DTTypes.h \
    src/DTHttpDownloaderImpl.h \
    src/DTDownloaderVimeo.h \
    src/DTDownloaderDailymotion.h \
    src/DTDownloaderFacebook.h \
    src/DTDownloaderMegavideo.h \
    src/DTDownloaderMegavideoScript.h \
    src/DTDownloaderMetacafe.h \
    src/DTIOService.h \
    src/DTDownloaderYoutubeTools.h \
    src/DTHttpDownloaderInstance.h
            
SOURCES += \
    src/DTDownloaderUtils.cpp \
    src/DTDownloaderXnxx.cpp \
    src/DTDownloaderYoutube.cpp \
    src/DTDownloaderYoutubeDetails.cpp \
    src/DTHttpDownloader.cpp \
    src/DTMediaDownloader.cpp \
    src/DTMediaSite.cpp \
    src/DTSimpleMediaParser.cpp \
    src/DTHttpDownloaderSimple.cpp \
    src/DTHttpDownloaderSimple2.cpp \
    src/DTHttpDownloaderMulti.cpp \
    src/DTDownloaderVimeo.cpp \
    src/DTDownloaderDailymotion.cpp \
    src/DTDownloaderFacebook.cpp \
    src/DTAudioPacket.cpp \
    src/DTAudioDataCreate.cpp \
    src/DTPacketSpecial.cpp \
    src/DTAudioEncoderSettingsImpl.cpp \
    src/DTAudioEncoderQueue.cpp \
    src/DTDownloaderMegavideo.cpp \
    src/DTDownloaderMegavideoScript.cpp \
    src/DTServiceScript.cpp \
    src/DTServiceScriptUrl.cpp \
    src/DTDownloaderMetacafe.cpp \
    src/DTIOService.cpp \
    src/DTDownloaderYoutubeTools.cpp \
    src/DTHttpDownloaderImpl.cpp

DEPENDENCY_LIBS = \
    sdkcommon \
    urdl

include ($$SOURCE_TREE/dependencies.pri)

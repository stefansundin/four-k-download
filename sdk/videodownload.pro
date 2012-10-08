TARGET = sdkvideodownload
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
    src/DTDownloaderYoutube.h \
    src/DTDownloaderYoutubeDetails.h \
    src/DTMediaDownloaderDetails.h \
    src/DTSimpleMediaParser.h \
    include/openmedia/DTMediaDownloader.h \
    src/DTMediaDownloaderImpl.h \
    include/openmedia/DTMediaSite.h \
    src/DTDownloaderVimeo.h \
    src/DTDownloaderDailymotion.h \
    src/DTDownloaderFacebook.h \
    src/DTDownloaderMetacafe.h \
    src/DTDownloaderYoutubeTools.h \
    src/videodownload/DTSubtitleImpl.h \
    src/videodownload/DTSubtitleDownload.h \
    src/videodownload/DTSubtitle.h \
    src/videodownload/DTSubtitleYoutube.h \
    src/videodownload/DTDownloaderRaiTv.h    
    
SOURCES += \
    src/DTDownloaderYoutube.cpp \
    src/DTDownloaderYoutubeDetails.cpp \
    src/DTMediaDownloader.cpp \
    src/DTMediaSite.cpp \
    src/DTSimpleMediaParser.cpp \
    src/DTDownloaderVimeo.cpp \
    src/DTDownloaderDailymotion.cpp \
    src/DTDownloaderFacebook.cpp \
    src/DTDownloaderMetacafe.cpp \
    src/DTDownloaderYoutubeTools.cpp \
    src/videodownload/DTSubtitle.cpp \
    src/videodownload/DTSubtitleDownload.cpp \
    src/videodownload/DTSubtitleYoutube.cpp \
    src/videodownload/DTDownloaderRaiTv.cpp
    
!include($$BUILD_TREE/config.pri) {
    error("Please run configure script");
    #also fails if .qmake.cache was not generated which may
    #happen if we are trying to shadow build w/o running configure
}

TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += urdl
SUBDIRS += sdk_common
SUBDIRS += sdk_download
SUBDIRS += sdk_media
SUBDIRS += sdk_examples
SUBDIRS += app_common
SUBDIRS += app_videodownloader
SUBDIRS += app_audiodownloader

urdl.file = 3rdparty/urdl/urdl.pro

sdk_common.file = sdk/common.pro
sdk_common.depends = \
    urdl

sdk_download.file = sdk/download.pro
sdk_download.depends = \
    urdl

sdk_media.file = sdk/media.pro
sdk_media.depends = \
    urdl

sdk_examples.file = sdk/examples/examples.pro
sdk_examples.depends = \
    sdk_common \
    sdk_download \
    sdk_media

app_common.file = app/common/common.pro

app_videodownloader.file = app/videodownloader/videodownloader.pro
app_videodownloader.depends = \
    app_common \
    sdk_common \
    sdk_download \
    sdk_media

app_audiodownloader.file = app/audiodownloader/audiodownloader.pro
app_audiodownloader.depends = \
    app_common \
    sdk_common \
    sdk_download \
    sdk_media

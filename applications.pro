!include($$BUILD_TREE/config.pri) {
    error("Please run configure script");
    #also fails if .qmake.cache was not generated which may
    #happen if we are trying to shadow build w/o running configure
}

TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += urdl
SUBDIRS += portaudiocxx
SUBDIRS += sdk_common
SUBDIRS += sdk_download
SUBDIRS += sdk_videodownload
SUBDIRS += sdk_media
SUBDIRS += sdk_examples
SUBDIRS += app_componentmodel
SUBDIRS += app_guicxx
SUBDIRS += app_guiqml
SUBDIRS += app_multimedia
SUBDIRS += app_mvvmcxx
SUBDIRS += app_mvvmqml
SUBDIRS += app_videodownloader
SUBDIRS += app_audiodownloader
SUBDIRS += app_videotomp3

urdl.file = 3rdparty/urdl/urdl.pro

portaudiocxx.file = 3rdparty/portaudio++/portaudio++.pro

sdk_common.file = sdk/common.pro
sdk_common.depends = \
    urdl

sdk_download.file = sdk/download.pro
sdk_download.depends = \
    sdk_common \
    urdl
    
sdk_videodownload.file = sdk/videodownload.pro
sdk_videodownload.depends = \
    sdk_common \
    sdk_download \
    urdl
    
sdk_media.file = sdk/media.pro
sdk_media.depends = \
    sdk_common \
    urdl
    
sdk_examples.file = sdk/examples/examples.pro
sdk_examples.depends = \
    sdk_common \
    sdk_download \
    sdk_media \
    portaudiocxx
    
app_componentmodel.file = app/common/componentmodel.pro

app_guicxx.file = app/common/guicxx.pro
app_guicxx.depends = \
    app_componentmodel

app_guiqml.file = app/common/guiqml.pro
app_guiqml.depends = \
    app_componentmodel

app_multimedia.file = app/common/multimedia.pro
app_multimedia.depends = \
    sdk_media \
    sdk_common \
    portaudioxx

app_mvvmcxx.file = app/common/mvvmcxx.pro
app_mvvmcxx.depends = \
    app_componentmodel

app_mvvmqml.file = app/common/mvvmqml.pro
app_mvvmqml.depends = \
    app_componentmodel
    
app_videodownloader.file = app/videodownloader/videodownloader.pro
app_videodownloader.depends = \
    app_componentmodel \
    app_feedback \
    app_guicxx \
    app_mvvmcxx \
    sdk_common \
    sdk_download \
    sdk_media

app_audiodownloader.file = app/audiodownloader/audiodownloader.pro
app_audiodownloader.depends = \
    app_componentmodel \
    app_feedback \
    app_guicxx \
    app_mvvmcxx \
    app_multimedia \
    sdk_common \
    sdk_download \
    sdk_media

app_videotomp3.file = app/videotomp3/videotomp3.pro
app_videotomp3.depends = \
    app_componentmodel \
    app_feedback \
    app_guiqml \
    app_mvvmqml \
    app_multimedia \
    sdk_common \
    sdk_download \
    sdk_media

TARGET = sdkcommon
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
    src/DTAllocator.h \
    src/DTMemoryBuffer.h \
    src/DTUtils.h \
    include/openmedia/DTAssert.h \
    include/openmedia/DTCommon.h \
    include/openmedia/DTConfig.h \
    include/openmedia/DTCStdInt.h \
    include/openmedia/DTDeclareImpl.h \
    include/openmedia/DTError.h \
    include/openmedia/DTPlatform.h \
    include/openmedia/DTTypes.h \
    include/openmedia/DTString.h \
    include/openmedia/DTScript.h \
    src/md5/md5.h \
    src/DTFOpen.h \
    src/fake_stream.h

SOURCES += \
    src/DTError.cpp \
    src/DTCommon.cpp \
    src/DTString.cpp \
    src/md5/md5.c \
    src/DTFOpen.cpp \
    src/fake_stream.cpp

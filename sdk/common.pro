TEMPLATE = lib
TARGET = sdkcommon

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
    src/DTAllocator.h \
    src/DTMemoryBuffer.h \
    src/DTUtils.h \
    include/openmedia/DTAssert.h \
    include/openmedia/DTCommon.h \
    include/openmedia/DTConfig.h \
    include/openmedia/DTCStdInt.h \
    include/openmedia/DTDeclareImpl.h \
    include/openmedia/DTError.h \
    include/openmedia/DTHeaders.h \
    include/openmedia/DTPlatform.h \
    include/openmedia/DTTypes.h \
    include/openmedia/DTString.h \
    include/openmedia/DTScript.h \
    src/DTPythonScript.h \
    src/DTScriptImpl.h \
    src/DTQtScript.h \
    src/md5/md5.h

SOURCES += \
    src/DTError.cpp \
    src/DTCommon.cpp \
    src/DTString.cpp \
    src/DTScript.cpp \
    src/DTScriptCreate.cpp \
    src/DTQtScript.cpp \
    src/md5/md5.c

DEPENDENCY_LIBS = \
    urdl

include ($$SOURCE_TREE/dependencies.pri)

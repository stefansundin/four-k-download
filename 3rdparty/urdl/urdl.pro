TARGET = urdl
TEMPLATE = lib
CONFIG += staticlib

QT -= core gui

DEFINES += URDL_DISABLE_SSL
DEFINES += BOOST_ALL_NO_LIB
DEFINES += URDL_STATIC_LINK=1 
win32 {
    DEFINES += _WIN32_WINNT=0x0501
    DEFINES += WIN32_LEAN_AND_MEAN=1 
}
DEFINES += BOOST_LOG_BUILDING_THE_LIB=1

QMAKE_CXXFLAGS += -fno-strict-aliasing


include ($$SOURCE_TREE/common.pri)


INCLUDEPATH += \
    $$BOOST_PATH/include \
    include

SOURCES += \
    src/urdl.cpp

HEADERS += \
    include/urdl/url.hpp \
    include/urdl/read_stream.hpp \
    include/urdl/option_set.hpp \
    include/urdl/istreambuf.hpp \
    include/urdl/istream.hpp \
    include/urdl/http.hpp \
    include/urdl/impl/url.ipp \
    include/urdl/impl/option_set.ipp \
    include/urdl/impl/istreambuf.ipp \
    include/urdl/impl/http.ipp \
    include/urdl/detail/parsers.hpp \
    include/urdl/detail/http_read_stream.hpp \
    include/urdl/detail/handshake.hpp \
    include/urdl/detail/file_read_stream.hpp \
    include/urdl/detail/coroutine.hpp \
    include/urdl/detail/connect.hpp \
    include/urdl/detail/config.hpp \
    include/urdl/detail/abi_suffix.hpp \
    include/urdl/detail/abi_prefix.hpp

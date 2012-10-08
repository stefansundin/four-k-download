######################################################################
#
# Common QMake settings
#
######################################################################

include($$BUILD_TREE/config.pri)


# Figure out the root of where stuff should go (this could be done via configure)
OUTPUT_DIR = $$BUILD_TREE
SOURCE_DIR = $$PWD

CONFIG(debug, debug|release) {
    SUBDIRPART=debug
} else {
    SUBDIRPART=release
}


OBJECTS_DIR = $$OUTPUT_DIR/build/$$SUBDIRPART/$$TARGET

contains(TEMPLATE,.*lib) {
    DESTDIR = $$OUTPUT_DIR/lib
} else {
    DESTDIR = $$OUTPUT_DIR/bin
}

MOC_DIR = $$OUTPUT_DIR/build/$$SUBDIRPART/$$TARGET/moc
RCC_DIR = $$OUTPUT_DIR/build/$$SUBDIRPART/$$TARGET/rcc
UI_DIR  = $$OUTPUT_DIR/build/$$SUBDIRPART/$$TARGET/ui

LIBS += -L$$OUTPUT_DIR/lib

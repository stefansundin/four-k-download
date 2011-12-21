#!/bin/sh
#
# Configures to build the project
#

#-------------------------------------------------------------------------------
# script initialization
#-------------------------------------------------------------------------------

SOURCE_PATH=`dirname $0`
SOURCE_PATH=`(cd "$SOURCE_PATH"; /bin/pwd)`
BUILD_PATH=`/bin/pwd`

PROJECT="$SOURCE_PATH/applications.pro"


#-------------------------------------------------------------------------------
# save configuration into config.pri
#-------------------------------------------------------------------------------

PROJECT_CONFIG="$BUILD_PATH/config.pri"

[ -f "$PROJECT_CONFIG" ] && rm -f "$PROJECT_CONFIG"

cat >>"$PROJECT_CONFIG" <<EOF
CONFIG += release
EOF


#-------------------------------------------------------------------------------
# save configuration into .qmake.cache
#-------------------------------------------------------------------------------

QMAKE_CACHE="$BUILD_PATH/.qmake.cache"

[ -f "$QMAKE_CACHE" ] && rm -f "$QMAKE_CACHE"

cat >>"$QMAKE_CACHE" <<EOF
SOURCE_TREE = \$\$quote($SOURCE_PATH)
BUILD_TREE = \$\$quote($BUILD_PATH)
EOF


#-------------------------------------------------------------------------------
# run qmake
#-------------------------------------------------------------------------------

echo Running qmake...
match="Darwin"
if [ `uname -s` = "${match}" ]; then
qmake $PROJECT -spec macx-g++
else
qmake $PROJECT
fi 
echo configure has finished. You may run \"make\" to build the project now.

#!/usr/bin/env bats

UTILS_BASE=$(realpath ./)
source scripts/util.bash
source languages/cc.bash

@test "init" {
    workspace=$(mktemp -d)
    mkdir -p $workspace
    init "$workspace"

    [ -e $workspace/src/CMakeLists.txt ]
    [ $(readlink $workspace/src/CMakeLists.txt) == "$UTILS_BASE/languages/cc/workspace/CMakeLists.txt" ]

    rm -rf $workspace
}

@test "create_newfile" {
    tmpfile=$(mktemp)
    workspace=$(mktemp -d)
    mkdir -p $workspace/build
    export BUILD_DIR=$workspace/build
    touch $workspace/CMakeLists.txt
    (create_newfile $tmpfile)

    expected=$(mktemp)
    cat << EOF > $expected
/*
URL https://
SCORE 0
AC false
WA false
TLE false
MLE false
TASK_TYPE
FAILURE_TYPE
NOTES
*/
EOF
    cat $UTILS_BASE/languages/cc/templates/src/common.cc $UTILS_BASE/languages/cc/templates/src/main.cc >> $expected

    [ "$(cat $tmpfile)" = "$(cat $expected)" ]

    rm -rf $tmpfile
    rm -rf $expected
    rm -rf $workspace
}
@test "execute" {
    BUILD_DIR=$(mktemp -d)

    cat <<EOF > $BUILD_DIR/Makefile
test:
	echo "Building CXX object" # dummy CMake message
	mkdir -p $BUILD_DIR/src
	echo "cat /dev/stdin" > $BUILD_DIR/src/test
	chmod u+x $BUILD_DIR/src/test
EOF

    [ "$(echo xxx | execute test)" = "$(echo xxx)" ]

    [ "$(echo xxx | execute test :)" = "" ]

    rm -rf $BUILD_DIR
}

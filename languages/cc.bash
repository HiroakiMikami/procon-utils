#!/bin/bash

function create_newfile() {
    set -u
    local target=$1

    local utils_base=${UTILS_BASE:-$(dirname $0)}
    local build_dir=${BUILD_DIR:-./build}

    mkdir -p $(dirname $target)
    cat << EOF > $target
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
    cat $utils_base/languages/cc/templates/src/common.cc \
        $utils_base/languages/cc/templates/src/main.cc   >> $target

    # Update cmake build dir
    local prev=$(pwd)
    mkdir -p $build_dir
    cd $build_dir
    cmake ..
    cd $prev
}

function execute() {
    set -u
    local path=$1
    shift 1

    local executable=$(echo ${path%.*} | tr "/" "-")
    local build_dir=${BUILD_DIR:-./build}

    # Build executable
    make -C $build_dir $executable 1>&2 &&
        $@ $build_dir/src/$executable < /dev/stdin
}

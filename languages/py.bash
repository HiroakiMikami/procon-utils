#!/bin/bash

function init() {
    local target=$1
    local utils_base=${UTILS_BASE:-$(dirname $0)}
}

function create_newfile() {
    set -u
    local target=$1

    local utils_base=${UTILS_BASE:-$(dirname $0)}
    local build_dir=${BUILD_DIR:-./build}

    mkdir -p $(dirname $target)
    touch $target
}

function execute() {
    set -u
    local path=$1
    shift 1

    $@ python $path
}

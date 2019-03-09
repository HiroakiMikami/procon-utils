#!/bin/bash

function contest_name() {
    set -u
    local path=$1

    echo $path | cut -f 3 -d "/"
}
function task_name() {
    set -u
    local path=$(basename $1)
    echo ${path%.*}
}

function login() {
    set -u

    local utils_base=${UTILS_BASE:-$(dirname $0)}
    local cachedir=${CACHEDIR}/atcoder
    mkdir -p $cachedir

    echo -n "Username: "
    read user_name
    echo -n "Password: "
    read password

    mkdir -p ${HOME}/.procon-utils/
    node ${utils_base}/services/atcoder/out/index.js \
        ${CONFIGDIR}/atcoder-session.json \
        $cachedir \
        login \
        $user_name $password
}

function task_url() {
    set -u

    local utils_base=${UTILS_BASE:-$(dirname $0)}
    local cachedir=${CACHEDIR}/atcoder
    mkdir -p $cachedir

    local path=$1
    local contest=$(contest_name $path)
    local task=$(task_name $path)

    mkdir -p ${HOME}/.procon-utils/
    node ${utils_base}/services/atcoder/out/index.js \
        ${CONFIGDIR}/atcoder-session.json \
        $cachedir \
        task_url \
        $contest $task
}
function score() {
    set -u

    local utils_base=${UTILS_BASE:-$(dirname $0)}
    local cachedir=${CACHEDIR}/atcoder
    mkdir -p $cachedir

    local path=$1
    local contest=$(contest_name $path)
    local task=$(task_name $path)

    mkdir -p ${HOME}/.procon-utils/
    node ${utils_base}/services/atcoder/out/index.js \
        ${CONFIGDIR}/atcoder-session.json \
        $cachedir \
        score \
        $contest $task
}

function testcases() {
    set -u
    local utils_base=${UTILS_BASE:-$(dirname $0)}
    local cachedir=${CACHEDIR}/atcoder
    mkdir -p $cachedir

    local path=$1
    local contest=$(contest_name $path)
    local task=$(task_name $path)
    local target=$2

    mkdir -p ${HOME}/.procon-utils/
    node ${utils_base}/services/atcoder/out/index.js \
        ${CONFIGDIR}/atcoder-session.json \
        $cachedir \
        testcases \
        $contest $task $target
}
function submit_results() {
    set -u
    set -u
    local utils_base=${UTILS_BASE:-$(dirname $0)}
    local cachedir=${CACHEDIR}/atcoder
    mkdir -p $cachedir

    local path=$1
    local contest=$(contest_name $path)
    local task=$(task_name $path)

    mkdir -p ${HOME}/.procon-utils/
    node ${utils_base}/services/atcoder/out/index.js \
        ${CONFIGDIR}/atcoder-session.json \
        $cachedir \
        submit_results \
        $contest $task
}

function tasks() {
    set -u
    local contest=$(contest_name $1)
    case $contest in
        abc*)
            echo "a b c d"
            ;;
        arc*)
            echo "c d e f"
            ;;
        *)
            echo "a b c d e f"
            ;;
    esac
}
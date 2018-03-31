#!/bin/bash

function login() {
    set -u
    dump "Not implemented yet"
    exit 1
}

function task_url() {
    set -u

    local aoj_domain=${AOJ_DOMAIN:-"http://judge.u-aizu.ac.jp"}

    local path=$1
    local base=$(basename $path)
    local id=${base%.*}
    echo ${aoj_domain}/onlinejudge/description.jsp?id=$id
}
function score() {
    set -u
    echo 0
}

function testcases() {
    set -u

    local path=$1
    local target=$2

    local tmp=$(http aoj GET $(task_url $path) | sed -e 's|<h3>Example 1</h3>|<example><h3>Example 1</h3>|' | \
                    query "count(//example/pre)" -)
    local num_of_samples=$(( $tmp / 2 ))
    local target=$2

    mkdir -p $target
    for i in $(seq 1 $num_of_samples)
    do
        in=$(( $i * 2 - 1 ))
        out=$(( $i * 2 ))
        http aoj GET $(task_url $path) | sed -e 's|<h3>Example 1</h3>|<example><h3>Example 1</h3>|' | \
            query "(//example/pre)[$in]/text()" - > $target/sample$i.in
        http aoj GET $(task_url $path) | sed -e 's|<h3>Example 1</h3>|<example><h3>Example 1</h3>|' | \
            query "(//example/pre)[$out]/text()" - > $target/sample$i.out
    done

    echo $num_of_samples
}
function submit_results() {
    set -u

    dump "Not implemented yet"
    exit 1
}

function tasks() {
    set -u
    echo $1
}

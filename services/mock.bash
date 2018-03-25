#!/bin/bash

function login() {
    set -u

    echo -n "Username: "
    read user_name
    echo -n "Password: "
    read password

    dump "login with ($user_name, $password)"
}

function task_url() {
    set -u
    local path=$1
    echo "https://mock/$path"
}
function score() {
    echo 0
}

function testcases() {
    set -u

    local path=$1
    local target=$2

    mkdir -p $target
    for i in $(seq 0 2)
    do
        echo "in$i" > $target/sample$i.in
        echo "out$i" > $target/sample$i.out
    done
    echo 2
}
function submit_results() {
    echo "false true true false "
}

function tasks() {
    echo "1 2 3 4"
}
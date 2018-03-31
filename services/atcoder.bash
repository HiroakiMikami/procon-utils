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

function task_key() {
    set -u

    local atcoder_domain=${ATCODER_DOMAIN:-"https://beta.atcoder.jp"}

    local contest_name=$1
    local task_name=$2

    local href=$(http atcoder GET $atcoder_domain/contests/$contest_name/tasks | \
                 query "//table/tbody/tr/td/a[text() = '${task_name^^}']/@href" -)
    local href=${href##*/}
    echo ${href%"\""}
}

function login() {
    set -u

    local atcoder_domain=${ATCODER_DOMAIN:-"https://beta.atcoder.jp"}

    echo -n "Username: "
    read user_name
    echo -n "Password: "
    read password

    cat << EOF > ${HOME}/.w3m/pre_form
url /beta.atcoder.jp/login
text username $user_name
password password $password
EOF
    chmod 600 ${HOME}/.w3m/pre_form
    w3m ${atcoder_domain}/login
}

function task_url() {
    set -u

    local atcoder_domain=${ATCODER_DOMAIN:-"https://beta.atcoder.jp"}

    local path=$1
    local contest=$(contest_name $path)
    local task=$(task_name $path)
    echo ${atcoder_domain}/contests/$contest/tasks/$(task_key $contest $task)
}
function score() {
    set -u

    local atcoder_domain=${ATCODER_DOMAIN:-"https://beta.atcoder.jp"}

    local path=$1

    http atcoder GET $(task_url $path) | query "//span[@class = 'lang-ja']/p/var/text()" -
}

function testcases() {
    set -u

    local atcoder_domain=${ATCODER_DOMAIN:-"https://beta.atcoder.jp"}

    local path=$1
    local target=$2

    local tmp=$(http atcoder GET $(task_url $path) | \
               query "count(//span[@class = 'lang-ja']/div[not(contains(@class, 'io-style'))]//pre)" -)
    local num_of_samples=$(( $tmp / 2 ))

    mkdir -p $target
    for i in $(seq 1 $num_of_samples)
    do
        in=$(( $i * 2 - 1 ))
        out=$(( $i * 2 ))
        http atcoder GET $(task_url $path) | \
            query "(//span[@class = 'lang-ja']/div[not(contains(@class, 'io-style'))]//pre)[$in]/text()" -  | \
            sed -e 's/\r//g' > $target/sample$i.in
        http atcoder GET $(task_url $path) | \
            query "(//span[@class = 'lang-ja']/div[not(contains(@class, 'io-style'))]//pre)[$out]/text()" - | \
            sed -e 's/\r//g' > $target/sample$i.out
    done

    echo $num_of_samples
}
function submit_results() {
    set -u

    local atcoder_domain=${ATCODER_DOMAIN:-"https://beta.atcoder.jp"}

    local path=$1
    
    local contest=$(contest_name $path)
    local task=$(task_name $path)
    local key=$(task_key $contest $task)

    local result=$(http atcoder GET "$atcoder_domain/contests/$contest/submissions/me?f.Task=$key" | \
                   query "//table/tbody/tr/td/span/text()" -)
    
    # is AC
    if echo $result | grep AC > /dev/null
    then
        echo -n "true "
    else
        echo -n "false "
    fi

    # WA
    if echo $result | grep WA > /dev/null
    then
        echo -n "true "
    else
        echo -n "false "
    fi

    # TLE
    if echo $result | grep TLE > /dev/null
    then
        echo -n "true "
    else
        echo -n "false "
    fi

    # MLE
    if echo $result | grep MLE > /dev/null
    then
        echo -n "true "
    else
        echo -n "false "
    fi
    
    echo
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
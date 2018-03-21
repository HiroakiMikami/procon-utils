#!/bin/bash

function dump() {
    set -u
    local fname=${BASH_SOURCE[1]##*/}
    echo -e "$(date '+%Y-%m-%dT%H:%M:%S') ${fname}:${BASH_LINENO[0]}:${FUNCNAME[1]} $@" >&2
}

function switch_w3m_config() {
    set -u
    local service_name=$1
    local source=${W3MDIR}/${service_name}
    mkdir -p $source

    tmpdir=$(mktemp -d)

    function on_finish() {
        set -u
        rm -rf $1
        mv ${HOME}/.w3m $1
        mv $tmpdir/.w3m ${HOME}/.w3m
        rm -rf $tmpdir
    }
    mv ${HOME}/.w3m $tmpdir/.w3m
    mv $source ${HOME}/.w3m
    trap "on_finish $source" EXIT
}

function http() {
    set -u
    local service_name=$1
    local method=$2
    local url=$3
    shift 3
    local args=$@

    # Use cache if it's possible
    local key=$(echo "$method-$url" | base64 | tr -d '\n')
    local cache_file=$CACHEDIR/$key
    if [ -e "$cache_file" ]
    then
        dump "use cache for $url"
        cat "$cache_file"
        return 0
    fi

    dump "make http request for $url"
    case $method in
        "GET")
            w3m -dump_source $url $args | gunzip -f | sed -e 's/\r//g' | tee "$cache_file"
            ;;
        *)
            dump "Method $method is not supported"
            exit 1
            ;;
    esac
}

function query() {
    set -u
    local q=$1
    local source=$2
    xmllint --html --xpath "$q" $source 2> /dev/null
}

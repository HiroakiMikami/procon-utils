#!/bin/bash

function init() {
    set -u
    local target=$1
    echo $target
}

function create_newfile() {
    set -u
    local target=$1

    mkdir -p $(dirname $target)
    cat << EOF > $target
#!/bin/bash
: << END
METADATA BEGIN
URL https://
SCORE 0
AC false
WA false
TLE false
MLE false
TASK_TYPE
FAILURE_TYPE
NOTES
METADATA END
END

EOF

    chmod u+x $target
}

function execute() {
    set -u
    local path=$1
    shift 1

    $@ $path < /dev/stdin
}

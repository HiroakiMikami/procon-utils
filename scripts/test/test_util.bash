#!/usr/bin/env bats

source scripts//util.bash

@test "http uses cache" {
    export CACHEDIR=$(mktemp -d)

    tmpfile=$(mktemp)
    echo "test" > $tmpfile
    result1=$(http mock GET file://$tmpfile)
    [ $result1 == "test" ]

    rm $tmpfile
    result2=$(http mock GET file://$tmpfile)
    [ $result1 == "test" ]

    rm -rf $CACHEDIR
}
@test "http cache for long name file" {
    export CACHEDIR=$(mktemp -d)

    tmpfile=$(mktemp --suffix=$(seq -s "," 0 20))
    echo "test" > $tmpfile
    result1=$(http mock GET file://$tmpfile)
    [ $result1 == "test" ]

    rm $tmpfile
    result2=$(http mock GET file://$tmpfile)
    [ $result1 == "test" ]

    rm -rf $CACHEDIR
}
@test "http remove \r" {
    export CACHEDIR=$(mktemp -d)

    tmpfile=$(mktemp)

    echo -e "test\r\n" > $tmpfile
    [ $(http mock GET file://$tmpfile) = $(echo -e "test\n") ]

    rm -rf $CACHEDIR
    rm -rf $tmpfile
}

@test "switch_w3m_config" {
    export W3MDIR=$(mktemp -d)
    mkdir -p $W3MDIR/tmp
    echo test > $W3MDIR/tmp/cookie

    tmpdir=$(mktemp -d)
    cp -r ${HOME}/.w3m $tmpdir/.w3m

    result=$(switch_w3m_config tmp && cat ${HOME}/.w3m/cookie)
    [ "$result" == "test" ]
    diff -r $tmpdir/.w3m/ ${HOME}/.w3m/
    retval=$?
    [ $retval -eq 0 ]

    rm -rf $W3MDIR
    rm -rf $tmpdir
}

@test "query" {
    tmpfile=$(mktemp)

    echo "<html><head><title>test</title></head></html>" > $tmpfile
    [ $(query "//title/text()" $tmpfile) = "test" ]

    rm -rf $tmpfile
}

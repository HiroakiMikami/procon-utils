#!/usr/bin/env bats

source scripts/util.bash
source services/aoj.bash

@test "task_url" {
    [ "$(task_url src/aoj/0001/0001.cc)" == "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=0001" ]
}
@test "testcases" {
    export CACHEDIR=$(mktemp -d)
    TMPDIR=$(mktemp -d)
    OUTPUT=$(mktemp -d)
    export AOJ_DOMAIN="file://$TMPDIR"

    mkdir -p $TMPDIR/onlinejudge/
    cp ./services/test/aoj/dummy_0001.html $TMPDIR/onlinejudge/description.jsp

    [ "$(testcases src/aoj/0001/0001.cc $OUTPUT)" -eq 2 ]

    echo 0 > $OUTPUT/expected
    [ "$(cat $OUTPUT/sample1.in)" = "$(cat $OUTPUT/expected)" ]
    echo 1 > $OUTPUT/expected
    [ "$(cat $OUTPUT/sample1.out)" = "$(cat $OUTPUT/expected)" ]
    echo 0 > $OUTPUT/expected
    echo 1 2 >> $OUTPUT/expected
    [ "$(cat $OUTPUT/sample2.in)" = "$(cat $OUTPUT/expected)" ]
    echo 1 > $OUTPUT/expected
    echo 2 >> $OUTPUT/expected
    [ "$(cat $OUTPUT/sample1.out)" = "$(cat $OUTPUT/expected)" ]

    rm -rf $TMPDIR
    rm -rf $OUTPUT
    rm -rf $CACHEDIR
}

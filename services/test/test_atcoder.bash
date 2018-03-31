#!/usr/bin/env bats

source scripts/util.bash
source services/atcoder.bash

@test "contest_name" {
    result=$(contest_name src/atcoder/arc000/a.cc)
    [ "$result" == "arc000" ]
    result=$(contest_name src/atcoder/arc000)
    [ "$result" == "arc000" ]
    result=$(contest_name src/atcoder/arc000/)
    [ "$result" == "arc000" ]
}
@test "task_name" {
    result=$(task_name src/atcoder/arc000/a.cc)
    [ "$result" == "a" ]
} 

@test "task_key" {
    export CACHEDIR=$(mktemp -d)
    TMPDIR=$(mktemp -d)
    export ATCODER_DOMAIN="file://$TMPDIR"

    mkdir -p $TMPDIR/contests/arcxxx
    cp ./services/test/atcoder/dummy_tasks.html $TMPDIR/contests/arcxxx/tasks
    echo $TMPDIR/contests/arcxxx/tasks
    [ "$(task_key arcxxx a)" == "arcxxx_a" ]
    [ "$(task_key arcxxx b)" == "arcxxx_b" ]
    [ "$(task_key arcxxx c)" == "arcxxx_c" ]
    [ "$(task_key arcxxx d)" == "arcxxx_d" ]

    rm -rf $TMPDIR
    rm -rf $CACHEDIR
}
@test "task_url" {
    export CACHEDIR=$(mktemp -d)
    TMPDIR=$(mktemp -d)
    export ATCODER_DOMAIN="file://$TMPDIR"

    mkdir -p $TMPDIR/contests/arcxxx
    cp ./services/test/atcoder/dummy_tasks.html $TMPDIR/contests/arcxxx/tasks
    [ "$(task_url src/atcoder/arcxxx/a.cc)" == "file://$TMPDIR/contests/arcxxx/tasks/arcxxx_a" ]

    rm -rf $TMPDIR
    rm -rf $CACHEDIR
}
@test "score" {
    echo ${BATS_TEST_SKIPPED}
    export CACHEDIR=$(mktemp -d)
    TMPDIR=$(mktemp -d)
    export ATCODER_DOMAIN="file://$TMPDIR"

    function task_key() {
        echo arcxxx_a
    }
    export -f task_key
    mkdir -p $TMPDIR/contests/arcxxx/tasks
    cp ./services/test/atcoder/dummy_arcxxx_a.html $TMPDIR/contests/arcxxx/tasks/arcxxx_a

    [ "$(score src/atcoder/arcxxx/a.cc)" == "0" ]

    rm -rf $TMPDIR
    rm -rf $CACHEDIR
}
@test "testcases" {
    export CACHEDIR=$(mktemp -d)
    TMPDIR=$(mktemp -d)
    OUTPUT=$(mktemp -d)
    export ATCODER_DOMAIN="file://$TMPDIR"

    function task_key() {
        echo arcxxx_a
    }
    export -f task_key
    mkdir -p $TMPDIR/contests/arcxxx/tasks
    cp ./services/test/atcoder/dummy_arcxxx_a.html $TMPDIR/contests/arcxxx/tasks/arcxxx_a

    [ "$(testcases src/atcoder/arcxxx/a.cc $OUTPUT)" -eq 2 ]

    echo 0 > $OUTPUT/expected
    [ "$(cat $OUTPUT/sample1.in)" = "$(cat $OUTPUT/expected)" ]
    echo 1 > $OUTPUT/expected
    [ "$(cat $OUTPUT/sample1.out)" = "$(cat $OUTPUT/expected)" ]
    echo 0 > $OUTPUT/expected
    echo 1 2 >> $OUTPUT/expected
    [ "$(cat $OUTPUT/sample2.in)" = "$(cat $OUTPUT/expected)" ]
    echo 1 > $OUTPUT/expected
    echo 2 >> $OUTPUT/expected
    [ "$(cat $OUTPUT/sample2.out)" = "$(cat $OUTPUT/expected)" ]

    rm -rf $TMPDIR
    rm -rf $OUTPUT
    rm -rf $CACHEDIR
}
@test "submit_results" {
    export CACHEDIR=$(mktemp -d)
    TMPDIR=$(mktemp -d)
    export ATCODER_DOMAIN="file://$TMPDIR"

    function task_key() {
        echo arcxxx_a
    }
    export -f task_key
    mkdir -p $TMPDIR/contests/arcxxx/submissions/
    cp ./services/test/atcoder/dummy_submissions.html $TMPDIR/contests/arcxxx/submissions/me

    [ "$(submit_results src/atcoder/arcxxx/a.cc)" = "true true false false " ]

    rm -rf $TMPDIR
    rm -rf $CACHEDIR
}
@test "tasks" {
    [ "$(tasks src/atcoder/abcxxx)" = "a b c d" ]
    [ "$(tasks src/atcoder/arcxxx)" = "c d e f" ]
    [ "$(tasks src/atcoder/agcxxx)" = "a b c d e f" ]
}

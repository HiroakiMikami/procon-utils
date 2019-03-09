#!/usr/bin/env bats

UTILS_BASE=$(realpath ./)
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
@test "tasks" {
    [ "$(tasks src/atcoder/abcxxx)" = "a b c d" ]
    [ "$(tasks src/atcoder/arcxxx)" = "c d e f" ]
    [ "$(tasks src/atcoder/agcxxx)" = "a b c d e f" ]
}

#!/bin/bash

for file in $(ls scripts/test/*.bash)
do
    echo $file
    $file || exit 1
    echo
    echo
done

for file in $(ls services/test/*.bash)
do
    echo $file
    $file || exit 1
    echo
    echo
done

for file in $(ls languages/test/*.bash)
do
    echo $file
    $file || exit 1
    echo
    echo
done

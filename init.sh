#! /bin/sh

root=$1

if [ -z "${root}" ]
then
    root=$(pwd)
fi

mkdir -p ${root}/src
ln -sf $(pwd)/workspace/CMakeLists.txt $(realpath ${root})/src/

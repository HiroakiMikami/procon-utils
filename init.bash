#! /bin/bash

root=$1

if [ -z "${root}" ]
then
    root=$(pwd)
fi

# Compile typescript
cd $(dirname $0)/services/atcoder
npm install .
npm run compile

cd -

# Create symlink of CMakeLists.txt (TODO)
ln -sf $(realpath $(dirname $0))/languages/cc/workspace/CMakeLists.txt $(realpath $root)/src

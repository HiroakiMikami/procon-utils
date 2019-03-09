#! /bin/sh

root=$1

if [ -z "${root}" ]
then
    root=$(pwd)
fi

# Compile typescript
cd $(dirname $0)/services/atcoder
npm run compile

cd -

#! /bin/sh

pip3 install --user online-judge-tools

cd $(dirname $0)/tools
npm install

oj login https://atcoder.jp/

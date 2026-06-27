#!/bin/bash

set -e

rm -rf oclint

# --branch also use tag
OCLINT_TAG=v26.02
git clone --single-branch --depth 1 --branch $OCLINT_TAG https://github.com/oclint/oclint.git oclint

head -n -11 oclint-rules/rules/CMakeLists.txt > temp.txt && mv temp.txt oclint-rules/rules/CMakeLists.txt \
echo "ADD_RULE_CATEGORY_DIRECTORY(moevmrules)" >> oclint-rules/rules/CMakeLists.txt
mv rules oclint-rules/rules/moevmrules

cd oclint/oclint-scripts

LLVM_PATH=/usr/lib/llvm-21
./build -llvm-root=$LLVM_PATH -release -clean core
./build -llvm-root=$LLVM_PATH -release -clean metrics
./build -llvm-root=$LLVM_PATH -release -clean driver
./build -llvm-root=$LLVM_PATH -release -clean reporters

mkdir -p ../build/oclint-rules/rules.dl
./bundle -llvm-root=$LLVM_PATH -release

cd ..

cp -r build/oclint-release/lib/* /usr/local/lib/
cp -r build/oclint-release/bin/* /usr/local/bin/


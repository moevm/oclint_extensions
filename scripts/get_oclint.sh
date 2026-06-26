#!/bin/bash

set -e

rm -rf oclint

# --branch also use tag
OCLINT_TAG=v22.02
git clone --single-branch --depth 1 --branch $OCLINT_TAG https://github.com/oclint/oclint.git oclint

cd oclint/oclint-scripts
./makeWithSystemLLVM /usr/lib/llvm-18

cd ..

# cp -r build/llvm-install /usr/local/llvm    # no need after build

cp -r build/oclint-release/lib/* /usr/local/lib/
cp -r build/oclint-release/bin/* /usr/local/bin/
cp -r build/oclint-core/lib/* /usr/local/lib/
cp -r build/oclint-metrics/lib/* /usr/local/lib/
cp -r build/oclint-rules/lib/* /usr/local/lib/

cp -r oclint-core/include/* /usr/local/include/
cp -r oclint-metrics/include/* /usr/local/include/
cp -r oclint-rules/include/* /usr/local/include/

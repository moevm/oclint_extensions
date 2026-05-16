#!/bin/bash

set -e

rm -rf oclint

# --branch also use tag
OCLINT_TAG=v22.02
git clone --single-branch --depth 1 --branch $OCLINT_TAG https://github.com/oclint/oclint.git oclint

cd oclint/oclint-scripts
./make

cd ..
cp -r build/oclint-release/lib/* /usr/local/lib/
cp -r build/oclint-release/bin/* /usr/local/bin/

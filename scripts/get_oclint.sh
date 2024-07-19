#!/bin/bash

set -e

OCLINT_22_02_COMMIT=d776db51c8574df406b2b0dc1b43b0b9b2d86d34
git clone https://github.com/oclint/oclint.git oclint
cd oclint
git reset --hard $OCLINT_22_02_COMMIT

cd oclint-scripts
./make

cd ..
cp -r build/oclint-release/lib/* /usr/local/lib/
cp -r build/oclint-release/bin/* /usr/local/bin/

#!/bin/bash

OCLINT_22_02_COMMIT=d776db51c8574df406b2b0dc1b43b0b9b2d86d34

if [ ! -d oclint ]; then
    git clone https://github.com/oclint/oclint.git oclint
    cd oclint
    git reset --hard $OCLINT_22_02_COMMIT
    cd ..
fi

cd oclint/oclint-scripts
./make

#!/bin/bash

if [ ! -d oclint ]; then
    git clone https://github.com/oclint/oclint.git oclint
    cd oclint
    git reset --hard d776db51c8574df406b2b0dc1b43b0b9b2d86d34
    cd ..
fi

cd oclint/oclint-scripts
./make
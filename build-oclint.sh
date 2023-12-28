#!/bin/bash

if [ ! -d oclint ]; then
    git clone https://github.com/oclint/oclint.git
fi

cd oclint/oclint-scripts
./make
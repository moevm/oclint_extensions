#!/usr/bin/bash

OLD_PATH=$(pwd)
SCRIPT_PATH=$(dirname "$0")

cd $SCRIPT_PATH

sudo ./install-deps.sh
./build-oclint.sh
./install-oclint.sh
mkdir build
cd build
cmake ../
cmake --build .
sudo cmake --install .
cd ..

cd $OLD_PATH

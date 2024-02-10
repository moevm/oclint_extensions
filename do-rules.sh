#!/usr/bin/bash

OLD_PATH=$(pwd)
SCRIPT_PATH=$(dirname "$0")

cd $SCRIPT_PATH

[[ -d build ]] || (mkdir build && cd build && cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ../ && cd ..)

cd build
cmake --build .
sudo cmake --install .
cp compile_commands.json ../
cd ..

cd $OLD_PATH

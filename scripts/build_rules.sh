#!/bin/bash

set -e

mkdir build
cd build
cmake ../ -DCMAKE_CXX_FLAGS="-Wl,--default-symver" -DCMAKE_SHARED_LINKER_FLAGS="-Wl,--default-symver"
cmake --build .
cmake --install . --prefix /usr/local

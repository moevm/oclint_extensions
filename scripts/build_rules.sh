#!/bin/bash

set -e

mkdir build
cd build
cmake ../
cmake --build .
cmake --install .

#!/bin/bash

DIRECTORY=solution
SRC_FILES=$(find $DIRECTORY -name "*.c") $(find $DIRECTORY -name "*.cpp")

echo "Files to check: $SRC_FILES"

oclint $SRC_FILES $@ --

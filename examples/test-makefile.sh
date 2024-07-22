#!/bin/bash

if [ $# -eq 0 ]; then
	echo "specify directory"
	exit 1
fi

WORK_DIRECTORY=$1
TMP_DIRECTORY=./tester-tmp

mkdir $TMP_DIRECTORY
cp -r $WORK_DIRECTORY/* $TMP_DIRECTORY

bear -- make -C $TMP_DIRECTORY > /dev/null

oclint ${@:2} \
	   $(find $TMP_DIRECTORY -name "*.c") \
	   $(find $TMP_DIRECTORY -name "*.h") \
	   $(find $TMP_DIRECTORY -name "*.cpp")

rm -rf $TMP_DIRECTORY
rm -rf compile_commands.json

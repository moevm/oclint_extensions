#!/bin/bash

if [ $# -eq 0 ]; then
	echo "specify directory"
	exit 1
fi

WORK_DIRECTORY=$1
SRC_FILE=$(find $WORK_DIRECTORY -name "*.c") \
		 $(find $WORK_DIRECTORY -name "*.cpp")

oclint ${@:2} $SRC_FILE --

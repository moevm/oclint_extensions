#!/bin/bash

set -e

PARAM_REPORT_TYPE="${PARAM_REPORT_TYPE:-text}"

cp -r solution tmp
chmod -R +x tmp
cd tmp

src_files=$(find . -name "*.c" && find . -name "*.cpp")
echo "Files to check:"
for file in $src_files; do
    echo "  * " $file
done

makefile=$(find . -name "Makefile")

if [ -z "$makefile" ]; then
    oclint $src_files $@ --report-type=${PARAM_REPORT_TYPE} --
else
    bear -- make > /dev/null
    oclint $src_files $@ --report-type=${PARAM_REPORT_TYPE}
fi

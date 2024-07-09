#!/bin/bash

cp -r solution tmp
chmod -R +x tmp
directory=tmp

src_files=$(find $directory -name "*.c" && find $directory -name "*.cpp")

echo "Files to check:"
for file in $src_files; do
    echo "  * " $(realpath -s --relative-to="$directory" "$file")
done

oclint $src_files $@ --

#!/usr/bin/bash
for i in $(find $1 -iname "*.c")
do
	dupfind --show $i
done

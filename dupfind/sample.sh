#!/usr/bin/bash
for i in $(find $1 -iname "*.c")
do
	./dupfind --similarity 90 --show $i
done

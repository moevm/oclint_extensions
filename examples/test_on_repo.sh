#!/usr/bin/bash

DIRS=$(find "$1" -maxdepth 1 -type d -name "*lb*")

mkdir -p out

for DIR in $DIRS
do
	NAME=$(basename $DIR)
	if [[ $(find $DIR -name "Makefile") ]]
	then
		./test-makefile.sh $DIR/src -report-type json > out/$NAME.json
	else
		./test-gcc.sh $DIR/src -report-type json > out/$NAME.json
	fi

	cat out/$NAME.json | python3 -m json.tool > tmp.json
	mv tmp.json out/$NAME.json
done

rm -f tmp.json
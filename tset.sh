#!/bin/sh
IDIR="/tmp/"		# input directory containing t-drive data set

if test "$1" = "0"; then
	echo 1
	./tdrive <$IDIR/3015.txt
	echo 10
	for i in `seq 10`; do
		echo 2 2 1000 1000 10 3600
		echo $i $i $i000 $i000 10 3600
	done
fi

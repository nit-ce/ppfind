#!/bin/sh
IDIR="/tmp/t-drive"		# input directory containing t-drive data set

if test "$1" = "0"; then
	echo 1
	./tdrive <$IDIR/3015.txt
	echo 10
	for i in `seq 10`; do
		echo 2 2 1000 1000 10 3600
		echo $i $i $i000 $i000 10 3600
	done
fi
if test "$1" = "1"; then
	./tdrive $IDIR/{6275,3015,3557,3579,366,8179,2560,8717,7146,1131,5860,4798,1277,2669,6665,8662,9109,5075,5099,3781,6656,9754,2237,7105,6211,8554,5070,8126,4177}.txt
	echo 100
	for i in `seq 100`; do
		echo 2 2 1000 1000 10 3600
		echo $i $i $i000 $i000 10 3600
	done
fi

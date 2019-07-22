#!/bin/sh
TDIR=/tmp/.ppfind

mkdir -p $TDIR

for x in 0 1 2 3 4
do
	./test1d $x | ./slow >$TDIR/t1
	./test1d $x | ./fast >$TDIR/t2
	cmp -s $TDIR/t1 $TDIR/t2 || echo "outputs differ for case $x"
done

for x in 11 12
do
	./test1d $x | ./slow 1 >$TDIR/t1
	./test1d $x | ./fast -d >$TDIR/t2
	cmp -s $TDIR/t1 $TDIR/t2 || echo "outputs differ for case $x"
done

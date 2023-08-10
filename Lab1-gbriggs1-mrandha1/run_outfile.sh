#!/bin/bash

# take the command line option for output file name or
# default "outfile" if no command line option given
if [ "$1" != "" ]; then
OUTFILE=$1
else
OUTFILE=/home/mrandha1/cs87/Lab1-gbriggs1-mrandha1/outfile
fi

for((n=512; n <= 1024; n=n*2))
do
  for ((t=2; t <= 32; t=t*2))
  do
    # often to print something around a set of related tests
    echo "***START*******" &>> $OUTFILE
    echo "/home/mrandha1/cs87/Lab1-gbriggs1-mrandha1/gol -t $t -n $n -m $n -k $i -x"
    for ((i=1; i<=5; i++))
    do
      echo "" &>> $OUTFILE
      echo "run $i:" &>> $OUTFILE
      (time /home/mrandha1/cs87/Lab1-gbriggs1-mrandha1/gol -t $t -n $n -m $n -k $i -x) &>> $OUTFILE
    done
    echo "***STOP********" &>> $OUTFILE
  done
done

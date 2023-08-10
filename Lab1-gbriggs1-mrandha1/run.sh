#!/bin/bash

for((n=1; n <= 2048; n=2*n))
do
  for ((t=1; t <= 32; t=t*2))
  do
    # often to print something around a set of related tests
    echo "***START*******"
    for ((i=1; i<=5; i++))
    do


        echo ""
        echo "/home/mrandha1/cs87/Lab1-gbriggs1-mrandha1/gol -t $t -n $n -m $n -k $i -x"
        echo "run $i:"
        time /home/mrandha1/cs87/Lab1-gbriggs1-mrandha1/gol -t $t -n $n -m $n -k $i -x



    done
    echo "***STOP********"
  done
done

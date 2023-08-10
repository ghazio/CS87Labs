#!/bin/bash

# take the command line option for output file name or
# default "outfile" if no command line option given
for((n=20; n <= 16000; n=2*n))
do
  # often to print something around a set of related tests
  echo "***START*******"
  echo "/home/mrandha1/cs87/Lab1-gbriggs1-mrandha1/gol -t 30 -n $n -m $n -k 500 -x"

  echo ""
  (time /home/mrandha1/cs87/Lab1-gbriggs1-mrandha1/gol -t 30 -n $n -m $n -k 500 -x)
  source /home/mrandha1/cs87/Lab1-gbriggs1-mrandha1/killmytests4.sh

  echo "***STOP********"
done

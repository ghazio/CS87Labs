#!/bin/bash

# take the command line option for output file name or
# default "outfile" if no command line option given

for ((t=100; t <= 10000; t=t*2))
do
  # often to print something around a set of related tests
  echo "***START*******"
  echo "/home/mrandha1/cs87/Lab1-gbriggs1-mrandha1/gol -t 30 -n 3000 -m 3000 -k $t -x"
  echo ""

  (time /home/mrandha1/cs87/Lab1-gbriggs1-mrandha1/gol -t 30 -n 3000 -m 3000 -k $t -x)
  source /home/mrandha1/cs87/Lab1-gbriggs1-mrandha1/killmytests2.sh
  echo "***STOP********"
done

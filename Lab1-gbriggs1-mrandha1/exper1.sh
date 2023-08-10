#!/bin/bash

# take the command line option for output file name or
# default "outfile" if no command line option given


# for((n=512; n <= 1024; n=n*2))
# do



  for ((t=4; t <= 1600; t=t*2))
  do
    # often to print something around a set of related tests
    echo "***START*******"
    echo "/home/mrandha1/cs87/Lab1-gbriggs1-mrandha1/gol -t $t -n 3000 -m 3000 -k 500 -x"

    echo ""
    (time /home/mrandha1/cs87/Lab1-gbriggs1-mrandha1/gol -t $t -n 3000 -m 3000 -k 500 -x)
    (source /home/mrandha1/cs87/Lab1-gbriggs1-mrandha1/killmytests1.sh)
    echo "***STOP********"
  done
# done

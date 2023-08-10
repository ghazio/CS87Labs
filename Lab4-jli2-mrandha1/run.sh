#!/bin/bash

# an example run script
#  * these are VERY SMALL runs, both data and process sizes are small)
#  * ./oddevensort is only running for default iters (may want to increase)

# To run:
#   use default hostfile:
#      ./run.sh
#   specify different hostfile to use as a command line arg to script:
#      ./run.sh  ./hostfilebig

# default hostfile
HOSTFILE=hostfile
OUTFILE1=/home/mrandha1/cs87/Lab4-jli2-mrandha1/results1.txt


# optionly run with a hostfile command line arg
if [[ $# -gt 0 ]]; then
  HOSTFILE=$1
fi

echo "using hostfile $HOSTFILE" #&>>$OUTFILE1
sleep 2


for((n=1024; n <= 9216; n=n*2))

do
  for ((i=1; i <= 64; i=i*2))
  do
    echo " " &>>$OUTFILE1
    echo "#################" #&>>$OUTFILE1
    echo "SIZE: $n, num_procs: $i"
    echo "SIZE: $n, num_procs: $i" &>>$OUTFILE1
    for ((j=1; j <= 5; j++))
    do
      echo "   run $j"
      echo "   run $j"&>>$OUTFILE1
      echo "time mpirun -np $i --hostfile $HOSTFILE ./oddevensort  $n ;"
      (time mpirun -np $i --hostfile $HOSTFILE ./oddevensort  $n ;) 2>> $OUTFILE1
    done
  done
done

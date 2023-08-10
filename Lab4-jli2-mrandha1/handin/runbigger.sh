#!/bin/bash

# default hostfile
HOSTFILE=hostfile
OUTFILE1=/home/mrandha1/cs87/Lab4-jli2-mrandha1/resultsbigger


# optionally run with a hostfile command line arg
if [[ $# -gt 0 ]]; then
  HOSTFILE=$1
fi

echo "using hostfile $HOSTFILE"

sleep 2

for((n=9216; n <= 147456; n=n*2))
do
for ((i=8; i <= 512; i=i*2))
do
  echo " "
  echo "SIZE: $n, num_procs: $i"
  echo "time mpirun -np $i --hostfile $HOSTFILE ./oddevensort  $n"
  time mpirun -np $i --hostfile $HOSTFILE ./oddevensort  $n
done
done

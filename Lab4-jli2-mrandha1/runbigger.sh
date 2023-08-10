#!/bin/bash

# default hostfile
HOSTFILE=hostfile
OUTFILE1=/home/mrandha1/cs87/Lab4-jli2-mrandha1/resultsbigger.txt


# optionally run with a hostfile command line arg
if [[ $# -gt 0 ]]; then
  HOSTFILE=$1
fi

echo "using hostfile $HOSTFILE"
echo "using hostfile $HOSTFILE" &>> $OUTFILE1


sleep 2

for((n=9216; n <= 147456; n=n*2))
#for((n=4; n <= 8; n=n*2))

do
for ((i=8; i <= 256; i=i*2))
#for ((i=16; i <= 32; i=i*2))

do
  echo " "
  echo "SIZE: $n, num_procs: $i"
  echo "SIZE: $n, num_procs: $i" &>>$OUTFILE1
  echo "time mpirun -np $i --hostfile $HOSTFILE ./oddevensort  $n"
  echo "time mpirun -np $i --hostfile $HOSTFILE ./oddevensort  $n" &>> $OUTFILE1

  (time mpirun -np $i --hostfile $HOSTFILE ./oddevensort  $n;) 2>> $OUTFILE1
done
done

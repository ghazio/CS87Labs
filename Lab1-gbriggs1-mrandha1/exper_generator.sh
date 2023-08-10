#!/bin/bash

# take the command line option for output file name or
# default "outfile" if no command line option given

OUTFILE1=/home/mrandha1/cs87/Lab1-gbriggs1-mrandha1/exper_generator_results1
OUTFILE2=/home/mrandha1/cs87/Lab1-gbriggs1-mrandha1/exper_generator_results2
OUTFILE3=/home/mrandha1/cs87/Lab1-gbriggs1-mrandha1/exper_generator_results3
OUTFILE4=/home/mrandha1/cs87/Lab1-gbriggs1-mrandha1/exper_generator_results4


# cp /dev/null $OUTFILE1
# cp /dev/null $OUTFILE2
cp /dev/null $OUTFILE3
# cp /dev/null $OUTFILE4





# echo "Experiment 1"&>> $OUTFILE1
# (source /home/mrandha1/cs87/Lab1-gbriggs1-mrandha1/exper1.sh &>> $OUTFILE1)

# echo "Experiment 2"&>> $OUTFILE2
# (source /home/mrandha1/cs87/Lab1-gbriggs1-mrandha1/exper2.sh &>> $OUTFILE2)

 cp /dev/null $OUTFILE3
 echo "Experiment 3"&>> $OUTFILE3
 (source /home/mrandha1/cs87/Lab1-gbriggs1-mrandha1/exper3.sh &>> $OUTFILE3)
#  echo "Done with experiment 3"
# #
# cp /dev/null $OUTFILE4
# echo "Experiment 4"&>> $OUTFILE4
# (source /home/mrandha1/cs87/Lab1-gbriggs1-mrandha1/exper4.sh &>> $OUTFILE4)

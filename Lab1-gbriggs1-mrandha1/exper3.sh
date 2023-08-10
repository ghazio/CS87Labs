#!/bin/bash

# take the command line option for output file name or
# default "outfile" if no command line option given

  # often to print something around a set of related tests
  echo "***START*******"
  echo "Row wise Parti/home/mrandha1/cs87/Lab1-gbriggs1-mrandha1/gol -t 30 -n 3000 -m 3000 -k 500 -x"
  echo ""

  (time /home/mrandha1/cs87/Lab1-gbriggs1-mrandha1/gol -t 30 -n 3000 -m 3000 -k 500 -x )
  source /home/mrandha1/cs87/Lab1-gbriggs1-mrandha1/killmytests3.sh
  echo ""
  echo ""

  echo "Coloumn wise Parti/home/mrandha1/cs87/Lab1-gbriggs1-mrandha1/gol -t 30 -n 3000 -m 3000 -k 500 -c"
  echo ""

  (time /home/mrandha1/cs87/Lab1-gbriggs1-mrandha1/gol  -t 30 -n 3000 -m 3000 -k 500 -x -c)
  source /home/mrandha1/cs87/Lab1-gbriggs1-mrandha1/killmytests3.sh


  # Experiment 3
  # ***START*******
  # Row wise Parti/home/mrandha1/cs87/Lab1-gbriggs1-mrandha1/gol -t 20 -n 500 -m 500 -k 500 -x
  #
  # rows: 500, cols : 500
  # number of alive cells processed: 9700343
  #
  # Total time: 2.387 seconds.
  #
  # real	0m2.406s
  # user	0m15.736s
  # sys	0m0.040s
  # Gone thy killing the tests
  #
  #
  # Coloumn wise Parti/home/mrandha1/cs87/Lab1-gbriggs1-mrandha1/gol -t 20 -n 500 -m 500 -k 500 -x -c
  #
  # rows: 500, cols : 500
  # number of alive cells processed: 9700343
  #
  # Total time: 2.413 seconds.
  #
  # real	0m2.426s
  # user	0m15.794s
  # sys	0m0.048s
  # Gone thy killing the tests
  # ***STOP********
  # Experiment in 8 core computer



  echo "***STOP********"

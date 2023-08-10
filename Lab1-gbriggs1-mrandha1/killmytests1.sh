#!/bin/bash
# kill my run script(s) first:
pkill -9 /home/mrandha1/cs87/Lab1-gbriggs1-mrandha1/exper_generator.sh
pkill -9 /home/mrandha1/cs87/Lab1-gbriggs1-mrandha1/exper1.sh
pkill -9 /home/mrandha1/cs87/Lab1-gbriggs1-mrandha1/exper2.sh
pkill -9 /home/mrandha1/cs87/Lab1-gbriggs1-mrandha1/exper3.sh
pkill -9 /home/mrandha1/cs87/Lab1-gbriggs1-mrandha1/exper4.sh




# then kill all my executables:
pkill -9 /home/mrandha1/cs87/Lab1-gbriggs1-mrandha1/gol

echo "I have killed all the children and hid their bodies :P "

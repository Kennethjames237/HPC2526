#!/bin/sh

N=1024
#N=2048

OPT="-O2"
#OPT="-O3"

HOSTNAME=$(hostname)
FILENAME=mm.$HOSTNAME.csv

gcc $OPT mm.c -lgsl -o mm

./mm    -n $N -c gcc-$HOSTNAME        > $FILENAME    # gcc       
./mm -g -n $N -c gcc+gsl-$HOSTNAME    >> $FILENAME   # gcc + gsl

#!/bin/bash

#SBATCH --output=%x.%j.out
#SBATCH --partition=cpu_guest
#SBATCH --qos=cpu_guest
#SBATCH --nodes=1
#SBATCH --cpus-per-task=32
#SBATCH --time=0-00:10:00
##SBATCH  --nodelist=wn21
##SBATCH  --exclude=wn20

gcc -O2 -fopenmp  omp_mm.c  -o  omp_mm

HOSTNAME=$(hostname)
FILENAME=omp_mm.$HOSTNAME.csv


#for N in 1024 2048
for N in 1024 
do
  for NT in 1 2 4 8
  do
    ./omp_mm -n $N -t $NT  -c $HOSTNAME-$NT 
  done
done   >   $FILENAME

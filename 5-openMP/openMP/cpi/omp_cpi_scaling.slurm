#!/bin/bash

#SBATCH --output=%x.%j.out
#SBATCH --partition=cpu_guest
#SBATCH --qos=cpu_guest
#SBATCH --nodes=1
#SBATCH --cpus-per-task=32
#SBATCH --time=0-00:10:00
##SBATCH  --nodelist=wn21
##SBATCH  --exclude=wn20

gcc -O2 -fopenmp  omp_cpi.c  -o  omp_cpi

for N in 1000000000 2000000000 4000000000 
do
  for NT in 1 2 4  8 16 32
  do
    ./omp_cpi -n $N -t $NT 
  done
done   >  omp_cpi_scaling.csv

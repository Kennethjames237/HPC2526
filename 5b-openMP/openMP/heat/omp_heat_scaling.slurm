#!/bin/bash

#SBATCH --output=%x.%j.out
#SBATCH --partition=cpu_guest
#SBATCH --qos=cpu_guest
#SBATCH --nodes=1
#SBATCH --cpus-per-task=16
#SBATCH --time=0-00:15:00

if command -v module;  then
  module load gnu8 
fi

gcc -O2 ./omp_heat.c -fopenmp -o omp_heat

S=500   # iterazioni

for N in 1024 2048 4096
#for N in 1024 2048 
do
 for NT in 1 2 4 8 16 
 do
 ./omp_heat -t $NT -c $N -r $N -s $S 1> /dev/null 
 done
done  2> omp_heat_scaling.csv

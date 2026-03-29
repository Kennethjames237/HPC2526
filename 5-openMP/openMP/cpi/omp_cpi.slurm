#!/bin/bash

#SBATCH --output=%x.%j.out
#SBATCH --partition=cpu_guest
#SBATCH --qos=cpu_guest
#SBATCH --nodes=1
#SBATCH --cpus-per-task=16
#SBATCH --time=0-00:10:00
##SBATCH  --nodelist=wn21
##SBATCH  --exclude=wn20


if [ -n "$SLURM_JOBID" ];
then
  module load gnu8
fi


gcc -O2 -fopenmp  omp_cpi.c  -o  omp_cpi

./omp_cpi -n 1000000000
./omp_cpi -n 1000000000 -t 8 
./omp_cpi -n 1000000000 -t 4

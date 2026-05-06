#!/bin/bash

#SBATCH --partition=cpu_guest
#SBATCH --qos=cpu_guest
#SBATCH --output=%x.%j.out
#SBATCH --nodes=1
#SBATCH --cpus-per-task=4
#SBATCH --time=0-24:00:00

#per generare un modulo da 68 bit:
#./omp_factorize -g 68

Modulus=B81915BC0A2222F4B         # 68

if [ -n "$SLURM_JOBID" ]; then
  module load gnu8
fi

gcc -O2 omp_factorize.c -o omp_factorize -lcrypto  -lm  -fopenmp

./omp_factorize -m $Modulus -a 8   > "$Modulus".txt

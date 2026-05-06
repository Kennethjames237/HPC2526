#!/bin/bash

#SBATCH --output=%x.%j.out
#SBATCH --partition=cpu_guest
#SBATCH --qos=cpu_guest
##SBATCH  --ntasks=4
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=2
#SBATCH --time=0-00:10:00
#SBATCH  --constraint=omnipath


if command -v module  # esecuzione sul cluster?
then
 module purge
 module load gnu8 openmpi4

 echo  "#SLURM_JOB_NODELIST      : $SLURM_JOB_NODELIST"
 echo  "#SLURM_NNODES            : $SLURM_NNODES"
 echo  "#SLURM_NTASKS            : $SLURM_NTASKS"
 echo  "#SLURM_JOB_CPUS_PER_NODE : $SLURM_JOB_CPUS_PER_NODE"
else
  NODELIST="-host wn1:4,wn2:4"
fi

mpicc -O2  mpi_cpi.c  -o  mpi_cpi -lm


for N in 1000000000 2000000000 4000000000 
do
  for NT in 1 2
  do
    mpirun  -npernode $NT  $NODELIST  mpi_cpi -n $N 
  done
done   > mpi_cpi-2n.csv


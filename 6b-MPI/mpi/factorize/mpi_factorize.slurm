#!/bin/bash

#SBATCH --partition=cpu_guest
#SBATCH --qos=cpu_guest
#SBATCH --output=%x.%j.out
#SBATCH --ntasks=4
##SBATCH --nodes=1
##SBATCH --ntasks-per-node=4
#SBATCH --time=0-00:05:00
#SBATCH --constraint=omnipath

Modulus=D1349980ADB5A74A19    # 72 bit


if command -v module
then
 module purge
 module load gnu8 openmpi4

 echo  "#SLURM_JOB_NODELIST      : $SLURM_JOB_NODELIST"
 echo  "#SLURM_NNODES            : $SLURM_NNODES"
 echo  "#SLURM_NTASKS            : $SLURM_NTASKS"
 echo  "#SLURM_JOB_CPUS_PER_NODE : $SLURM_JOB_CPUS_PER_NODE"
fi


# versione MPI  con supporto checkpoint
#mpicc -O2  mpi_factorize.c        -lcrypto -lm  -o  mpi_factorize
mpicc -O2  mpi_factorize_irecv.c   -lcrypto -lm  -o mpi_factorize_irecv
#mpirun     mpi_factorize           -m $Modulus -a 8    > "$Modulus".txt
mpirun     mpi_factorize_irecv     -m $Modulus -a 8    > "$Modulus".txt

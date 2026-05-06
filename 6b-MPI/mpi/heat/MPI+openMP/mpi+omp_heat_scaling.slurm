#!/bin/bash

#SBATCH --output=%x.%j.out           # Nome del file per lo strandard output
#SBATCH --partition=cpu_guest        # Nome della partizione
#SBATCH --qos=cpu_guest              # Nome della partizione
#SBATCH --nodes=2                    # numero di nodi richiesti
#SBATCH --ntasks-per-node=4          # numero di cpu per nodo
#SBATCH --cpus-per-task=1
#SBATCH --time=0-00:30:00            # massimo tempo di calcolo
#SBATCH --constraint=omnipath  

 module purge
 # module load gnu8 openmpi4
 module load intel impi

 echo  "#SLURM_JOB_NODELIST      : $SLURM_JOB_NODELIST"
 echo  "#SLURM_NNODES            : $SLURM_NNODES"
 echo  "#SLURM_NTASKS            : $SLURM_NTASKS"
 echo  "#SLURM_JOB_CPUS_PER_NODE : $SLURM_JOB_CPUS_PER_NODE"

 icc   heat.c         -o heat         -O2
 icc   omp_heat.c     -o omp_heat     -O2 -fopenmp
 mpicc mpi_heat.c     -o mpi_heat     -O2 -fopenmp
 mpicc mpi+omp_heat.c -o mpi+omp_heat -O2 -fopenmp

S=1000
#DIM=2048
#DIM=4096
DIM=8192

rm -f mpi+omp_heat_scaling.csv omp_heat_scaling.csv mpi_heat_scaling.csv heat.csv

./heat -c $DIM -r $DIM  -s $S    1> /dev/null  2>> heat.csv  # programma seriale

for T in 1 2 4 
do
  # T threads:
  ./omp_heat     -t $T -c $DIM -r $DIM  -s $S                           2>> omp_heat_scaling.csv 
  # T MPI task:
  mpirun -n $T      ./mpi_heat           -c $DIM -r $DIM  -s $S         2>> mpi_heat_scaling.csv
  # 2 nodi, 1 task per nodo, T thread per task:
  mpirun -ppn 1 $NODELIST ./mpi+omp_heat -t $T -c $DIM -r $DIM  -s $S   2>> mpi+omp_heat_scaling.csv 
done


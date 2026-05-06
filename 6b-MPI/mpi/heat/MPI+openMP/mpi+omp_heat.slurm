#!/bin/bash
#SBATCH --output=%x.%j.out
#SBATCH --partition=cpu_guest
#SBATCH --qos=cpu_guest
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=8
#SBATCH --time=0-00:05:00
#SBATCH --constraint=omnipath


 module purge
# module load gnu8 openmpi4
module load intel impi

 echo  "#SLURM_JOB_NODELIST      : $SLURM_JOB_NODELIST"
 echo  "#SLURM_NNODES            : $SLURM_NNODES"
 echo  "#SLURM_NTASKS            : $SLURM_NTASKS"
 echo  "#SLURM_JOB_CPUS_PER_NODE : $SLURM_JOB_CPUS_PER_NODE"

mpicc mpi+omp_heat.c -o mpi+omp_heat -O2 -fopenmp

for N in 2048 8192 
do
mpirun           ./mpi+omp_heat -c $N -r $N        # 2 task - 4 thread per task
mpirun  -ppn 2   ./mpi+omp_heat -c $N -r $N  -t 2  # 4 task - 2 thread per task
mpirun  -ppn 4   ./mpi+omp_heat -c $N -r $N  -t 1  # 8 task - 1 thread per task
done  2> mpi+omp_heat.csv

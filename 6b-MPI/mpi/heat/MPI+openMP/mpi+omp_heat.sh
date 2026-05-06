#!/bin/bash


mpicc mpi+omp_heat.c -o mpi+omp_heat -O2 -fopenmp

NODELIST="-host wn1:4,wn2:4"

for N in 2048 8192 
do
mpirun               $NODELIST ./mpi+omp_heat -c $N -r $N        # 2 task - 4 thread per task
mpirun  -npernode 2  $NODELIST ./mpi+omp_heat -c $N -r $N  -t 2  # 4 task - 2 thread per task
mpirun  -npernode 4  $NODELIST ./mpi+omp_heat -c $N -r $N  -t 1  # 8 task - 1 thread per task
done  2> mpi+omp_heat.csv

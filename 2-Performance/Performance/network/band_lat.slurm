#!/bin/sh

#SBATCH --job-name=band-lat     #
#SBATCH --output=%x.%j.out           # Nome del file per lo strandard output
#SBATCH --partition=cpu_guest        # Nome della partizione
#SBATCH --qos=cpu_guest              # 
#SBATCH --nodes=2                    # numero di nodi richiesti
#SBATCH --ntasks-per-node=2          # numero di cpu per nodo
##SBATCH --mem=2G                    # massima memoria utilizzata
#SBATCH --time=0-00:10:00 # massimo tempo di calcolo
#SBATCH --constraint=omnipath

echo  "#SLURM_JOB_NODELIST      : $SLURM_JOB_NODELIST"
echo  "#SLURM_JOB_CPUS_PER_NODE : $SLURM_JOB_CPUS_PER_NODE"


HOSTNAME=$(hostname)

module load gnu8 openmpi3
#module load intel impi
mpicc mpi_bandwidth.c -o mpi_bandwidth
mpicc mpi_latency.c  -o mpi_latency


mpirun -npernode 2 -np 2 mpi_bandwidth 2> band1-$HOSTNAME.csv  # 2 task 1 node
mpirun -npernode 1 -np 2 mpi_bandwidth 2> band2-$HOSTNAME.csv  # 1 task per node

mpirun -npernode 2 -np 2 mpi_latency   2> lat1-$HOSTNAME.csv   # 2 task 1 node
mpirun -npernode 1 -np 2 mpi_latency   2> lat2-$HOSTNAME.csv   # 1 task per node



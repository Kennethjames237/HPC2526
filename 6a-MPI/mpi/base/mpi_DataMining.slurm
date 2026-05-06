#!/bin/sh

#SBATCH --output=%x.%j.out           # Nome del file per lo strandard output
#SBATCH --partition=cpu_guest        # Nome della partizione
#SBATCH --qos=cpu_guest              # Nome della partizione
#SBATCH --nodes=2                    # numero di nodi richiesti 
#SBATCH --ntasks-per-node=2          # numero di task per nodo
#SBATCH --time=0-00:10:00            # massimo tempo di calcolo
#SBATCH --constraint=omnipath        # solo nodi con schede OmniPath
##SBATCH  --nodelist=wn21
##SBATCH  --exclude=wn20

if command -v module;
 then
 module purge
 module load  gnu8 openmpi4

 echo  "#SLURM_JOB_NODELIST      : $SLURM_JOB_NODELIST"
 echo  "#SLURM_NNODES            : $SLURM_NNODES"
 echo  "#SLURM_NTASKS            : $SLURM_NTASKS"
 echo  "#SLURM_JOB_CPUS_PER_NODE : $SLURM_JOB_CPUS_PER_NODE"
fi

mpicc mpi_DataMining.c -o  mpi_DataMining
mpirun ./mpi_DataMining

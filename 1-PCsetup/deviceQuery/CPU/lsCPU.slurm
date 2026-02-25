#!/bin/bash

#SBATCH --output=%x.%j.out   # Nome del file per lo strandard output
##SBATCH --error=%x.%j.err   # Se non specificato stderr e' rediretto su stdout
#SBATCH --partition=cpu_guest       # Nome della partizione
#SBATCH --qos=cpu_guest              # 
#SBATCH --nodes=1                    # numero di nodi richiesti
#SBATCH --cpus-per-task=1            # numero di cpu 
##SBATCH --mem=2G                    # massima memoria utilizzata
#SBATCH --time=0-00:02:00            # massimo tempo di calcolo
##SBATCH --account=T_2025_HPCPROGPAR  # Account da usare 
## Su login il comando hpc-show-user-account mostra gli account personali
## Per gli studenti del corso il default e' T_2025_HPCPROGPAR

# informazioni sull'ambiente Slurm

 echo SLURM_JOBID=$SLURM_JOBID
 echo SLURM_NODELIST=$SLURM_NODELIST
 echo SLURM_JOB_NAME=$SLURM_JOB_NAME
 echo SLURM_MEM_PER_CPU=$SLURM_MEM_PER_CPU 
 echo SLURM_NNODES=$SLURM_NNODES
 echo SLURM_TASKS_PER_NODE=$SLURM_TASKS_PER_NODE
 echo SLURM_JOB_USER=$SLURM_JOB_USER
 echo SLURM_SUBMIT_DIR=$SLURM_SUBMIT_DIR
 echo SLURM_CPUS_ON_NODE=$SLURM_CPUS_ON_NODE
 echo SLURM_JOB_CPUS_PER_NODE=$SLURM_JOB_CPUS_PER_NODE
 echo SLURM_JOB_CPUS_PER_NODE=$SLURM_JOB_PARTITION
 echo SLURM_JOB_ACCOUNT=$SLURM_JOB_ACCOUNT
 echo SLURM_JOB_NUM_NODES=$SLURM_JOB_NUM_NODES


# da eseguire su login per visualizzare
# i nodi disponibili nella partizione cpu_guest:
# hpc-sinfo-cpu --partition=cpu_guest


# informazioni sul nodo di esecuzione

hostname
lscpu
#numactl --hardware

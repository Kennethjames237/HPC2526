#!/bin/sh

#SBATCH --output=%x.%j.P100.out  # Nome del file per lo strandard output
##SBATCH --error=%x.%j.err  # Se non specificato stderr e' rediretto su stdout 
#SBATCH --partition=gpu
#SBATCH --qos=gpu
#SBATCH --nodes=1
#SBATCH --gres=gpu:1                 
#SBATCH --gres=gpu:p100:1             #  P100
##SBATCH --gres=gpu:a100_80g:1        #  A100 - 80 GBram
#SBATCH --time=0-0:02:00


# da eseguire su login per visualizzare
# i nodi disponibili nella partizione gpu:
# hpc-sinfo-gpu --partition=gpu

if command -v module
module load gnu/5.4.0 cuda/11.5.2
fi

hostname
nvidia-smi -L
./deviceQuery

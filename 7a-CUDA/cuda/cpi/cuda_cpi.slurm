#!/bin/sh
 
#SBATCH --output=%x.%j.out
#SBATCH --partition=gpu
#SBATCH --qos=gpu
#SBATCH --gres=gpu:p100:1
#SBATCH --time=0-00:05:00
#SBATCH --mem=12G


if command -v module
then
 module load cuda

 #stampa il nome del nodo assegnato e argomenti
 echo "#SLURM_JOB_NODELIST   : $SLURM_JOB_NODELIST"
 echo "#CUDA_VISIBLE_DEVICES : $CUDA_VISIBLE_DEVICES"
fi


threadsPerBlock=1024
nblocks=10240

nvcc cuda_cpi_global.cu -o cuda_cpi_global 
./cuda_cpi_global  -b $nblocks -t $threadsPerBlock

nvcc cuda_cpi_shared.cu -o cuda_cpi_shared
./cuda_cpi_shared  -b $nblocks -t $threadsPerBlock


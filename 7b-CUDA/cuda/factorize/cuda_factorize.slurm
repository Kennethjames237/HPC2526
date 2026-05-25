#!/bin/bash
 
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

nvcc cuda_factorize.cu -o cuda_factorize  -lcrypto -lm


Modulus=962193207571718113 # 1021963079 x 941514647

export TIMEFORMAT="%R"
{ time ./cuda_factorize -m $Modulus -t 1024 -b 2; }      >> $Modulus.txt  2>&1
{ time ./cuda_factorize -m $Modulus -t 1024 -b 1024; }   >> $Modulus.txt  2>&1
{ time ./cuda_factorize -m $Modulus -t 1024 -b 102400; } >> $Modulus.txt  2>&1

#!/bin/sh

#SBATCH --output=%x.%j.out
#SBATCH --partition=gpu
#SBATCH --nodes=1
#SBATCH --mem=4G
#SBATCH --qos=gpu
#SBATCH --gres=gpu:p100:1
##SBATCH --gres=gpu:a100_80g:1
#SBATCH --time=0-00:01:00

#stampa il nome del nodo assegnato e argomenti
echo "#SLURM_JOB_NODELIST   : $SLURM_JOB_NODELIST"
echo "#CUDA_VISIBLE_DEVICES : $CUDA_VISIBLE_DEVICES"


HOSTNAME=$(hostname)

if command -v module
then
module load cuda
fi

WA=1024
HA=1024
WB=1024
HB=1024

echo "size A= $WA X $HA,B= $WB X $HB"
echo "# N, gflop, tempo , GFLOPs,  Hostname , COMMENT" > mm_GPU.$HOSTNAME.csv

./matrixMul -wA=$WA -hA=$HA -wB=$WB -hB=$HB >> mm_GPU.$HOSTNAME.csv


#!/bin/sh

#SBATCH --output=%x.%j.out
#SBATCH --partition=gpu
#SBATCH --qos=gpu
#SBATCH --gres=gpu:p100:1
#SBATCH --time=0-00:02:00

if command -v module
then
 module load cuda 

 #stampa il nome del nodo assegnato e argomenti
 echo "#SLURM_JOB_NODELIST   : $SLURM_JOB_NODELIST"
 echo "#CUDA_VISIBLE_DEVICES : $CUDA_VISIBLE_DEVICES"
fi

nvcc cuda1_printf.cu  # in un kernel <<<10,10>>>  ogni thread stampa le proprie coordinate
#nvcc  cuda2_sumArray.cu   # 2 vettori di float sull'host vengono copiati su GPU, sommati e copiato da GPU a host il risultato 
#nvcc cuda3_matMul.cu     # 2 matrici NxN vengono copiate su GPU con 1 blocco 2D  e  moltiplicate 
#nvcc  cuda4_riduzione_blocco.cu  # float A[16] viene copiato sulla GPU che somma elementi (riduzione somma) e torna il risultato
#nvcc  cuda5_managed.cu    # il vettore Vett e' managed; la GPU calcola il quadrato di ogni elemento
#nvcc  cuda6_riduzione_griglia.cu # Sezione critica a livello warp o blocco  con atomicCAS e atomicExch
#nvcc   cuda7_syncblocks.cu # sincronismo tra i blocchi di una griglia

./a.out

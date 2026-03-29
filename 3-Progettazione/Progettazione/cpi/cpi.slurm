#!/bin/bash

#SBATCH --output=%x.%j.out           # Nome del file per lo strandard output
#SBATCH --partition=cpu_guest        # Nome della partizione, cpu per job paralleli
#SBATCH --qos=cpu_guest              # 
#SBATCH --nodes=1                    # numero di nodi richiesti
#SBATCH --cpus-per-task=1            # numero di cpu 
##SBATCH --mem=2G                    # massima memoria utilizzata
#SBATCH --time=0-00:10:00            # massimo tempo di calcolo

N=100000000

gcc cpi.c    -O2 -o cpi  -lm 
./cpi -n $N 

gcc cpi+amdahl.c  -O2 -o cpi+amdahl -lm 
./cpi+amdahl -n $N -s 100000  


#!/bin/sh

#SBATCH --output=%x.%j.out    # Nome del file per lo strandard output
##SBATCH --error=%x.%j.err    # Se non specificato stderr e' rediretto su stdout
#SBATCH --partition=cpu_guest    # Nome della partizione
#SBATCH --qos=cpu_guest              # 
#SBATCH --nodes=1                    # numero di nodi richiesti
#SBATCH --cpus-per-task=1            # numero di cpu
##SBATCH --mem=2G                    # massima memoria utilizzata
#SBATCH --time=0-00:05:00            # massimo tempo di calcolo

gcc -O2 timing.c -o timing

./timing > timing.csv


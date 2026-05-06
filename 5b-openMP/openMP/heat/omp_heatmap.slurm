#!/bin/bash

#SBATCH --output=%x.%j.out  # Nome del file per lo strandard output
#SBATCH --partition=cpu_guest        # Nome della partizione
#SBATCH --qos=cpu_guest
#SBATCH --nodes=1                    # numero di nodi richiesti
#SBATCH --cpus-per-task=6            # numero di cpu 
#SBATCH --time=0-00:10:00            # massimo tempo di calcolo

gcc -fopenmp -O2 omp_heat.c -o omp_heat

./omp_heat -r 1000 -c 1000      > heatmap.txt



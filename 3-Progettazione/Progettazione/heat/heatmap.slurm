#!/bin/sh

#SBATCH --output=%x.%j.out  # Nome del file per lo strandard output
#SBATCH --partition=cpu_guest        # Nome della partizione
#SBATCH --qos=cpu_guest
#SBATCH --nodes=1                    # numero di nodi richiesti
#SBATCH --cpus-per-task=1            # numero di cpu 
#SBATCH --time=0-00:10:00            # massimo tempo di calcolo

gcc -O2 heat.c -o heat

./heat >  heatmap.out



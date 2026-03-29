#!/bin/sh

#SBATCH --output=%x.%j.out  # Nome del file per lo strandard output
#SBATCH --partition=cpu_guest        # Nome della partizione
#SBATCH --qos=cpu_guest
#SBATCH --nodes=1                    # numero di nodi richiesti
#SBATCH --cpus-per-task=1            # numero di cpu 
#SBATCH --time=0-00:10:00            # massimo tempo di calcolo


# profiling di heat.c con l'opzione -pg
gcc  -O2 -pg heat.c -o heat-pg

#l'esecuzione scrive in gmon.out le info di profilazione
./heat-pg -r 4096 -c 4096 -s 500 > /dev/null

#Il comando gprof legge gmon.out e stampa i dati in stdout
gprof -b heat-pg  

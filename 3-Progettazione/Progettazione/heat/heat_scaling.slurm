#!/bin/sh

#SBATCH --output=%x.%j.out  # Nome del file per lo strandard output
#SBATCH --partition=cpu_guest        # Nome della partizione
#SBATCH --qos=cpu_guest
#SBATCH --nodes=1                    # numero di nodi richiesti
#SBATCH --cpus-per-task=1            # numero di cpu
#SBATCH --time=0-00:10:00            # massimo tempo di calcolo

echo "nx,  ny,  iter,  time,  jtime" > heat_scaling.csv

for N  in   512 758 1024 2048
do
./heat -c $N  -r $N -s 1000  > /dev/null # il file heatmap non viene creato
done   2>> heat_scaling.csv

 #2 standance for standard error in the that will go in the csv file(for this program)

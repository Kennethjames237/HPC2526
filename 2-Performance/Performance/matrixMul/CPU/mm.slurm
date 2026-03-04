#!/bin/bash
## da esguire sul cluster, non sull'host

## DIRETTIVE  PER SLURM
#SBATCH --output=%x.%j.out  # Nome del file per lo strandard output
##SBATCH --error=%x.%j.err  # Se non specificato stderr e' rediretto su stdout
#SBATCH --partition=cpu_guest        # Nome della partizione, cpu per job pargsleli
#SBATCH --qos=cpu_guest              # 
#SBATCH --nodes=1                    # numero di nodi richiesti
#SBATCH --cpus-per-task=16           # numero di cpu 
##SBATCH --mem=2G                    # massima memoria utilizzata
#SBATCH --time=0-00:10:00            # massimo tempo di calcolo

N=1024
#N=2048

OPT="-O2"
#OPT="-O3"
HOSTNAME=$(hostname)

FILENAME=mm.$HOSTNAME.csv

module purge
module load gnu8 gsl

gcc $OPT mm_gsl.c -lgsl    -o mm_gsl
gcc $OPT omp_mm.c -fopenmp -o omp_mm
./mm_gsl    -n $N -c gcc-$HOSTNAME        > $FILENAME   # gcc       
./mm_gsl -g -n $N -c gcc+gsl-$HOSTNAME    >> $FILENAME   # gcc + gsl
./omp_mm    -n $N -c gcc+omp-$HOSTNAME    >> $FILENAME   # gcc + openMP


module purge
module load intel 

icc $OPT mm_gsl.c -lgsl    -o mm_gsl
icc $OPT omp_mm.c -fopenmp -o omp_mm
./mm_gsl    -n $N  -c  icc-$HOSTNAME      >>  $FILENAME  # icc
./mm_gsl -g -n $N  -c  icc+gsl-$HOSTNAME  >>  $FILENAME  # icc + gsl
./omp_mm    -n $N  -c  gcc+omp-$HOSTNAME  >>  $FILENAME  # icc + omp

icc $OPT  -mavx -axCORE-AVX512  mm_gsl.c -lgsl  -o mm_gsl
./mm_gsl    -n $N  -c icc+avx-$HOSTNAME      >>  $FILENAME   # icc + AVX
./mm_gsl -g -n $N  -c icc+avx+gsl-$HOSTNAME  >>  $FILENAME   # icc + AVX + gsl

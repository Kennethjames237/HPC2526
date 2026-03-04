#!/bin/sh
 
#SBATCH --partition=cpu_guest
#SBATCH --qos=cpu_guest
##SBATCH --nodelist=wn18
#SBATCH --output=%x.%j.out
#SBATCH --nodes=1
#SBATCH --cpus-per-task=1
##SBATCH --mem=2G
#SBATCH --time=0-00:10:00 
 
echo "#SLURM_JOB_NODELIST: $SLURM_JOB_NODELIST"

DISKTEST=~/HPC2526/2-Performance/storage/disktest.sh

echo "####################################"
echo "# HOME perf"
cd $HOME
pwd
cp $DISKTEST .
sh disktest.sh

echo "####################################"
echo "# SCRATCH perf"
cd $SCRATCH
pwd
cp $DISKTEST .
sh disktest.sh

echo "####################################"
echo "# ARCHIVE perf"
cd $ARCHIVE
pwd
cp $DISKTEST .
sh disktest.sh

echo "####################################"



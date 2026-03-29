#!/bin/sh

#SBATCH --partition=cpu_guest
#SBATCH --qos=cpu_guest
#SBATCH --output=%x.%j.out

#SBATCH --nodes=1
#SBATCH --cpus-per-task=1
#SBATCH --time=0-12:00:00

#echo "#SLURM_JOB_NODELIST: $SLURM_JOB_NODELIST"
#module load gnu8

gcc factorize.c -o factorize -lcrypto  -lm

# time ./factorize -m B81915BC0A2222F4B  -a 4    # modulus 68 bit - prime 34 bit (4 addr + 30 block)
 time ./factorize -m B81915BC0A2222F4B  -a 8    # modulus 68 bit - prime 34 bit (8 addr + 26 block)

 #please compile with bash not sh


mpicc  mpi_heat_checkpoint.c -o mpi_heat_checkpoint

for N in 1024 
 do
  for NT in 1 2 4 6
  do
    mpirun -np $NT mpi_heat_checkpoint -c $N -r $N 1> /dev/null
  done
 done 

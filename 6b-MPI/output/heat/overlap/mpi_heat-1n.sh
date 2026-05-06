
mpicc -O2 mpi_heat_overlap.c -o mpi_heat_overlap

for N in 1024  2048 
do
 for NT in 1 2 4 
 do
 mpirun  -n  $NT mpi_heat_overlap -c $N -r $N 1> /dev/null
 done
done  2> mpi_heat_overlap-1n.csv


  NODELIST="-host wn1:4,wn2:4"

mpicc -O2 mpi_heat_overlap.c -o mpi_heat_overlap  

for N in 1024  2048 
do
 for NT in 1 2 4
 do
 mpirun  -npernode  $NT  $NODELIST  mpi_heat_overlap -c $N -r $N 1> /dev/null
 done
done  2> mpi_heat_overlap-2n.csv

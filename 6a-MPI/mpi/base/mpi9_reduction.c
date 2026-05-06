/*

*/

#include <stdio.h>
#include "mpi.h"

int main (int argc, char *argv[]) {
 MPI_Status status;
 int i, mpi_rank, mpi_size, reduction=0, received;


 MPI_Init(&argc, &argv);
 MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
 MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

if(mpi_rank==0) 
   printf("tramite MPI_Reduce() ogni task invia il proprio rank, il rank 0 riceve e somma\n"); 

MPI_Reduce(&mpi_rank, &reduction, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD); 

printf("Sono il task: %d \n", mpi_rank);

if (mpi_rank==0) 
   printf ("mpi_size: %d, reduction: %d \n", mpi_size,  reduction); 

 MPI_Finalize();
 return 0;
}

/*

*/

#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define LSIZE 10

int main (int argc, char *argv[]) {
 MPI_Status status;
 int i, mpi_rank, mpi_size, vsize;
 int * localVector, * Vector; 

 MPI_Init(&argc, &argv);
 MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
 MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

 printf("mpi_rank: %d, mpi_size %d \n", mpi_rank, mpi_size);

 vsize= LSIZE*mpi_size; 

 localVector=(int *)malloc(sizeof(int)*LSIZE);

 if (mpi_rank == 0) 
  {
    Vector=(int *)malloc(sizeof(int)*vsize);
    for (i=0; i < vsize; i++)  *(Vector+i)=0;
  }

MPI_Barrier(MPI_COMM_WORLD); 

if (mpi_rank == 0)
   {for (i=0; i < vsize ; i++) printf ("%d ", *(Vector+i));
     printf("\n"); }



MPI_Scatter(Vector, LSIZE, MPI_INT, localVector, LSIZE, MPI_INT,  0, MPI_COMM_WORLD);   

if (mpi_rank == 0) 
 printf("Ogni task riceve dati con MPI_scatter(). imposta il suo rank number  e rispedisce con MPI_gather()\n"); 

for (i=0; i < LSIZE ; i++) *(localVector+i)+=mpi_rank ; // ogni task imposta il suo rank number

MPI_Gather (localVector, LSIZE, MPI_INT, Vector, LSIZE, MPI_INT,  0, MPI_COMM_WORLD);   

 if (mpi_rank == 0)  
   {for (i=0; i < vsize ; i++) printf ("%d ", *(Vector+i));
     printf("\n"); }

 MPI_Finalize();
 return 0;
}

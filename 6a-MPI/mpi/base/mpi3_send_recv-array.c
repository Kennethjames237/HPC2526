#include <stdio.h>
#include "mpi.h"
#include <unistd.h>
#define MSIZE 10

int main(int argc, char **argv)
{
 MPI_Status status;
 int rank, size;
 int i, j;

 float a[MSIZE];   /*data to communicate */
 MPI_Init(&argc, &argv);
 MPI_Comm_rank(MPI_COMM_WORLD, &rank);
 if (rank == 0) {
  for (i = 0; i<MSIZE; i++)  a[i] = (float) i;
   printf("Process 0 sends the following array to process 1.\n");
   for (i = 0; i<MSIZE; i++)  printf("%6.2f ", a[i]); printf("\n");
  MPI_Send(a, MSIZE,MPI_FLOAT, 1, 666, MPI_COMM_WORLD);
  }
 else if (rank == 1) {
   usleep(500000); 
   MPI_Recv(a, MSIZE, MPI_FLOAT, 0, 666, MPI_COMM_WORLD, &status);
   printf("Process 1 receives the following array from process 0.\n");
   for (i = 0; i<MSIZE; i++)  printf("%6.2f ", a[i]); printf("\n"); 
  }
 MPI_Finalize();
 return 0;
}

#include <stdio.h>
#include <mpi.h>

int main (int argc, char *argv[])
 {
 int MPIrank, MPIsize;
 int send_buf, recv_buf;
 int prev, next, sum, i;

 MPI_Status status;
 MPI_Request request;
 MPI_Init(&argc, &argv);
 MPI_Comm_rank(MPI_COMM_WORLD, &MPIrank);
 MPI_Comm_size(MPI_COMM_WORLD, &MPIsize);
 
 if (MPIrank==0)
  printf("Ogni task determina il rank del task che precede (prev) e che segue (next) - MPIsize= %d\n", MPIsize); 

 next  = (MPIrank+1) % MPIsize;
 prev  = (MPIrank-1+MPIsize) % MPIsize;

 sum = 0;
 send_buf = MPIrank;
 for( i = 0; i < MPIsize; i++)
  {
 MPI_Sendrecv(&send_buf, 1, MPI_INT, next, 666,
              &recv_buf, 1, MPI_INT, prev, 666, MPI_COMM_WORLD, &status);

  sum += recv_buf;
  send_buf= recv_buf; 
  }
 printf ("Rank %d - Sum %d\n", MPIrank, sum);
 MPI_Finalize();
 }

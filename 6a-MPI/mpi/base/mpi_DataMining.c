/*
  mpi_DataMining.c
 Il task 0 genera un numero intero x random tra 0 e domain_size e tutti collaborano per  trovarlo.
IL task 0 invia il numero  a tutti con boadcast.
Il dominio viene  diviso a grana fine e ogni task analizza il proprio sottominio.

mpicc mpi_DataMining.c -o mpi_DataMining
mpirun  mpi_DataMining
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <mpi.h>

int main (int argc, char *argv[]) {
 MPI_Status status;
 MPI_Request request;
 int  MPIrank, MPIsize;
 int idx, x, flag, found=0;
 int domain_size;

 MPI_Init(&argc, &argv);
 MPI_Comm_rank(MPI_COMM_WORLD, &MPIrank);
 MPI_Comm_size(MPI_COMM_WORLD, &MPIsize);

 domain_size = 128; 
 printf ("Task (%d %% %d = %d) domain %d  \n", MPIrank, MPIsize, MPIrank % MPIsize, domain_size);

 srand48((unsigned)time(NULL)); // inizializzazione del seme
 if (MPIrank==0) x=drand48()*domain_size+1;

 // il rank 0 condivide invia x agli altri task con un broadcast
 MPI_Bcast(&x, 1, MPI_INT, 0 , MPI_COMM_WORLD);

 // attiva la ricezione asincrona non bloccante di x
 MPI_Irecv(&found, 1 ,MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &request);
printf("Task (%d %% %d = %d) MPI_Irecv started\n",
       MPIrank, MPIsize, MPIrank % MPIsize);

 MPI_Barrier(MPI_COMM_WORLD);

 for (idx=MPIrank; idx <= domain_size ; idx=idx+MPIsize)
        {
        if (found)   break;
        printf("task %d  processing  %d \n", MPIrank, idx);
        if (idx == x)
            {
            printf ("Task %d found %d  \n", MPIrank, idx);
             
            for (int i=0; i<MPIsize ;i++) MPI_Send(&idx, 1, MPI_INT, i, 666, MPI_COMM_WORLD);  //  invio a tutti di r
            }
        usleep(200000);
        MPI_Test( &request, &flag, &status);
         }

 printf ("Task (%d %% %d = %d) - RECEIVED %d from %d, exiting \n", MPIrank, MPIsize, MPIrank % MPIsize, found, status.MPI_SOURCE);
 MPI_Finalize();

 return 0;
}

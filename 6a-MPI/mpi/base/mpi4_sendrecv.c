#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    MPI_Status status;
    MPI_Request request;
    int MPIrank, MPIsize;
    int prev, next;

    int send_buf, recv_buf; /* data to communicate */

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &MPIrank);
    MPI_Comm_size(MPI_COMM_WORLD, &MPIsize);

    prev = (MPIrank - 1 + MPIsize) % MPIsize;
    next = (MPIrank + 1) % MPIsize;

    send_buf = MPIrank;

    //  MPI_Isend(&send_buf, 1, MPI_INT, next, 666, MPI_COMM_WORLD, &request);
    //  MPI_Recv(&recv_buf,  1, MPI_INT, prev, 666, MPI_COMM_WORLD, &status);
    //  MPI_Wait(&request, &status);

    MPI_Sendrecv(&send_buf, 1, MPI_INT, next, 666,
                 &recv_buf, 1, MPI_INT, prev, 666, MPI_COMM_WORLD, &status);

   printf("Process (%d %% %d = %d) receives %d from process %d\n",
       MPIrank, MPIsize, MPIrank % MPIsize,
       recv_buf, status.MPI_SOURCE);

    MPI_Finalize();
    return 0;
}

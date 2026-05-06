#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <mpi.h>

void options(int argc, char *argv[]);
void Jacobi_Iterator_CPU(float *__restrict T, float *__restrict T_new, const int NX, const int NY);

void Init_top(float *__restrict h_T, const int NX, const int NY);
void print_heatmap(float *__restrict h_T, const int NX, const int NY);

void load_heatmap(float *__restrict h_T, const int NX, const int NY);
void save_heatmap(float *__restrict h_T, const int NX, const int NY, int iter);

// defaults
int WNX = 256;
int WNY = 256;
int MAX_ITER = 1000;

int NX, NY;

// MPI globals
int mpi_rank = 0, mpi_size = 0;
int prev_rank = 0, next_rank = 0;
int tag = 999;

char hostname[MPI_MAX_PROCESSOR_NAME];
int namelen;

int debug = 0;
int help = 0;

float *h_T_new;
float *h_T_old;
float *h_T_temp;
float *h_T_whole;

/******** MAIN ********/
int main(int argc, char **argv)
{
   int iter;
   double t1, t2;

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
   MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
   MPI_Get_processor_name(hostname, &namelen);

   options(argc, argv);

   if (help)
   {
      if (mpi_rank == 0)
         printf("\n%s [-c ncols] [-r nrows] [-s nsteps] [-d] [-h]\n", argv[0]);
      MPI_Finalize();
      return 0;
   }

   prev_rank = (mpi_rank - 1 + mpi_size) % mpi_size;
   next_rank = (mpi_rank + 1) % mpi_size;

   NX = WNX + 2;
   NY = WNY / mpi_size + 2;

   fprintf(stderr, "# WNX:%d WNY:%d MAX_ITER:%d RANK:%d SIZE:%d NX:%d NY:%d\n",
           WNX, WNY, MAX_ITER, mpi_rank, mpi_size, NX, NY);

   h_T_new = (float *)calloc(NX * NY, sizeof(float));
   h_T_old = (float *)calloc(NX * NY, sizeof(float));

   /******** LOAD CHECKPOINT (ONLY RANK 0) ********/
   if (mpi_rank == 0)
   {
      h_T_whole = (float *)calloc((WNX + 2) * (WNY + 2), sizeof(float));

      load_heatmap(h_T_whole, WNX, WNY); // ✅ REQUIRED

      if (debug)
      {
         printf("# Heatmap iniziale:\n");
         print_heatmap(h_T_whole, WNX, WNY);
      }
   }

   MPI_Scatter(&(h_T_whole[NX]), NX * (NY - 2), MPI_FLOAT,
               h_T_old, NX * (NY - 2), MPI_FLOAT,
               0, MPI_COMM_WORLD);

   t1 = MPI_Wtime();

   for (iter = 0; iter < MAX_ITER; iter++)
   {
      if (mpi_rank == 0)
      {
         Init_top(h_T_old, NX, NY);
      }

      MPI_Sendrecv(&(h_T_old[NX * (NY - 2)]), NX, MPI_FLOAT, next_rank, tag,
                   &(h_T_old[0]), NX, MPI_FLOAT, prev_rank, tag,
                   MPI_COMM_WORLD, MPI_STATUS_IGNORE);

      MPI_Sendrecv(&(h_T_old[NX]), NX, MPI_FLOAT, prev_rank, tag,
                   &(h_T_old[NX * (NY - 1)]), NX, MPI_FLOAT, next_rank, tag,
                   MPI_COMM_WORLD, MPI_STATUS_IGNORE);

      Jacobi_Iterator_CPU(h_T_old, h_T_new, NX, NY);

      h_T_temp = h_T_new;
      h_T_new = h_T_old;
      h_T_old = h_T_temp;

      /******** CHECKPOINT SAVE ********/
      if (iter % 100 == 0)
      {

         MPI_Gather(&(h_T_old[NX]), NX * (NY - 2), MPI_FLOAT,
                    &(h_T_whole[NX]), NX * (NY - 2), MPI_FLOAT,
                    0, MPI_COMM_WORLD);

         if (mpi_rank == 0)
         {

            printf("Saving iteration %d\n", iter);

            save_heatmap(h_T_whole, WNX, WNY, iter); // ✅ REQUIRED
         }
      }
   }

   t2 = MPI_Wtime();

   if (mpi_rank == 0)
   {
      fprintf(stderr, "MPI time: %f\n", t2 - t1);
   }

   MPI_Gather(&(h_T_old[NX]), NX * (NY - 2), MPI_FLOAT,
              &(h_T_whole[NX]), NX * (NY - 2), MPI_FLOAT,
              0, MPI_COMM_WORLD);

   if (mpi_rank == 0 && debug)
   {
      printf("# Final heatmap:\n");
      print_heatmap(h_T_whole, WNX, WNY);
   }

   free(h_T_new);
   free(h_T_old);
   if (mpi_rank == 0)
      free(h_T_whole);

   MPI_Finalize();
   return 0;
}

/******** SAVE HEATMAP (CHECKPOINT) ********/
void save_heatmap(float *h_T, const int NX, const int NY, int iter)
{
   FILE *fp = fopen("heatmap.txt", "w");

   fprintf(fp, "ITERATION %d\n", iter);

   for (int j = 1; j < NY; j++)
   {
      for (int i = 1; i < NX; i++)
         fprintf(fp, "%.2f ", h_T[NX * j + i]);
      fprintf(fp, "\n");
   }

   fclose(fp);
}

/******** LOAD HEATMAP ********/
void load_heatmap(float *h_T, const int NX, const int NY)
{
   FILE *fp = fopen("heatmap.txt", "r");
   if (!fp)
      return;

   for (int j = 1; j < NY; j++)
      for (int i = 1; i < NX; i++)
         fscanf(fp, "%f", &h_T[NX * j + i]);

   fclose(fp);
}

/******** JACOBI ********/
void Jacobi_Iterator_CPU(float *T, float *T_new, const int NX, const int NY)
{
   for (int j = 1; j < NY - 1; j++)
      for (int i = 1; i < NX - 1; i++)
      {
         float E = T[(i + 1) + NX * j];
         float W = T[(i - 1) + NX * j];
         float N = T[i + NX * (j + 1)];
         float S = T[i + NX * (j - 1)];

         T_new[NX * j + i] = 0.25 * (E + W + N + S);
      }
}

/******** OPTIONS ********/
void options(int argc, char *argv[])
{
   int c;
   while ((c = getopt(argc, argv, "c:r:s:dh")) != -1)
   {
      switch (c)
      {
      case 'c':
         WNX = atoi(optarg);
         break;
      case 'r':
         WNY = atoi(optarg);
         break;
      case 's':
         MAX_ITER = atoi(optarg);
         break;
      case 'd':
         debug = 1;
         break;
      case 'h':
         help = 1;
         break;
      }
   }
}

/******** PRINT ********/
void print_heatmap(float *h_T, const int NX, const int NY)
{
   for (int j = 1; j < NY; j++)
   {
      for (int i = 1; i < NX; i++)
         printf("%.2f ", h_T[NX * j + i]);
      printf("\n");
   }
}

/******** INIT TOP ********/
void Init_top(float *h_T, const int NX, const int NY)
{
   for (int i = 1; i < NX - 1; i++)
      h_T[NX + i] = 1.0;
}
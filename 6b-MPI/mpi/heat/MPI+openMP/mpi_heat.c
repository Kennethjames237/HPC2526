/*
mpi_heat.c

module load gnu8 openmpi4
mpicc -O2 mpi_heat.c -o mpi_heat

mpirun mpi_heat -h
mpirun mpi_heat -r 32 -c 32 -s 4
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <mpi.h>

void options(int argc, char * argv[]) ;
void Jacobi_Iterator_CPU(float * __restrict T, float * __restrict T_new, const int NX, const int NY);
void Init_center(float * __restrict h_T, const int NX, const int NY); // center
void Init_left(float * __restrict h_T, const int NX, const int NY);   // left border
void Init_top(float * __restrict h_T, const int NX, const int NY);    // top border
void copy_rows(float * __restrict h_T, const int NX, const int NY);   // periodic boundary conditions
void copy_cols(float * __restrict h_T, const int NX, const int NY);   // periodic boundary conditions
void print_heatmap(float * __restrict h_T, const int NX, const int NY);
//
void load_heatmap(float * __restrict h_T,  const int NX, const int NY);
void save_heatmap(float * __restrict h_T,  const int NX, const int NY);
void Jacobi_Iterator_CPU_internal(float * __restrict T, float * __restrict T_new, const int NX, const int NY);
void Jacobi_Iterator_CPU_external(float * __restrict T, float * __restrict T_new, const int NX, const int NY);

// defaults
int WNX = 256;         // --- Number of discretization points along the x axis
int WNY = 256;         // --- Number of discretization points along the y axis
int MAX_ITER = 1000;  // --- Number of Jacobi iterations

int NX,NY;


// variabili globali per MPI
int mpi_rank=0, mpi_size=0;
int prev_rank=0, next_rank=0;
int tag = 999;
MPI_Status status;
char hostname[MPI_MAX_PROCESSOR_NAME]; 
int namelen;
int debug=0; 
int help=0; 

float *h_T_new;
float *h_T_old;
float *h_T_temp;
float *h_T_whole;

/********/
/* MAIN */
/********/
int main(int argc, char **argv)
{

   int iter,i,j;
   double t1, t2;

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
   MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
   MPI_Get_processor_name(hostname,&namelen);

   options(argc, argv);         /* optarg management */

   if (help)
   {
   if (mpi_rank==0)  printf ("\n%s [-c ncols] [-r nrows] [-s nsteps] [-d] [-h] \n",argv[0]); 
   MPI_Finalize(); 
   return (0);
   }

// scomposizione di dominio:
// dividiamo le righe tra i rank MPI
   prev_rank = (mpi_rank-1+mpi_size) % mpi_size;
   next_rank = (mpi_rank+1) % mpi_size;
   NX      =  WNX+2;                       // Local NX: numero colonne per rank
   NY      =  WNY/mpi_size+2;            // Local NY: numero righe   per rank

// stampa tutti i dati della simulazione
   fprintf (stderr,"# WNX:%d, WNY:%d, MAX_ITER:%d, MPI_RANK:%d, MPI_SIZE:%d, NX:%d, NY:%d, %s \n", WNX, WNY, MAX_ITER, mpi_rank, mpi_size, NX, NY, hostname);

   h_T_new          = (float *)calloc(NX * NY, sizeof(float));
   h_T_old          = (float *)calloc(NX * NY, sizeof(float));


// Rank 0 crea e inizializza  la matrice completa (Whole)
   if (mpi_rank == 0)  
     {
      h_T_whole  = (float *)calloc( (WNX+2) * (WNY+2), sizeof(float));
      if (debug)
       {
        printf("#Heatmap iniziale: \n"); 
        print_heatmap(h_T_whole,  WNX,  WNY);
       }
     }

   MPI_Scatter(&(h_T_whole[NX]), NX*(NY-2), MPI_FLOAT, h_T_old,  NX*(NY-2), MPI_FLOAT,  0,  MPI_COMM_WORLD);

  t1=MPI_Wtime();

  for(iter=0; iter<MAX_ITER; iter=iter+1)
    {
//     Init_center(h_T_old,  NX, NY);
//     Init_left(h_T_old,    NX, NY);
     if (mpi_rank==0)   Init_top(h_T_old,     NX, NY);
//     copy_cols(h_T_old, NX, NY);

// scambio delle righe di bordo

    // downward
   MPI_Sendrecv( &(h_T_old[NX*(NY-2)]), NX, MPI_FLOAT, next_rank, tag, //send
               &(h_T_old[NX*0]),      NX, MPI_FLOAT, prev_rank, tag, //recv
               MPI_COMM_WORLD, MPI_STATUS_IGNORE);
   // upward
   MPI_Sendrecv( &(h_T_old[NX*1]),    NX, MPI_FLOAT, prev_rank, tag, //send
               &(h_T_old[NX*(NY-1)]), NX, MPI_FLOAT, next_rank, tag, //recv
               MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    Jacobi_Iterator_CPU(h_T_old, h_T_new, NX, NY);

    h_T_temp=h_T_new;
    h_T_new=h_T_old;
    h_T_old=h_T_temp;

    }

  t2=MPI_Wtime();

  if(mpi_rank == 0) {  
     fprintf(stderr,"MPI, %d, %d, %d, %d,  %f\n", mpi_size,  WNX, WNY, MAX_ITER, t2-t1);
     fflush(stderr);
    }

  MPI_Gather(&(h_T_old[NX]), NX*(NY-2), MPI_FLOAT, &(h_T_whole[NX]),  NX*(NY-2), MPI_FLOAT,  0,  MPI_COMM_WORLD);

  if (mpi_rank == 0)
    {
     if (debug)
     {
      printf("#Heatmap finale: \n"); 
      print_heatmap(h_T_whole, WNX, WNY);
     }
    }

   free(h_T_new);
   free(h_T_old);
   if (mpi_rank == 0 ) free(h_T_whole);

   MPI_Finalize();

   return 0;
}


/***********************************/
/* JACOBI ITERATION FUNCTION - CPU */
/***********************************/
void Jacobi_Iterator_CPU(float * __restrict T, float * __restrict T_new, const int NX, const int NY)
{
int i,j;

        // --- Only update "interior" (not boundary) node points
        for(j=1; j<NY-1; j++)
            for(i=1; i<NX-1; i++) {
                float T_E = T[(i+1) + NX*j];
                float T_W = T[(i-1) + NX*j];
                float T_N = T[i + NX*(j+1)];
                float T_S = T[i + NX*(j-1)];
                T_new[NX*j + i] = 0.25*(T_E + T_W + T_N + T_S);
            }
}

/***********************************/
/* JACOBI ITERATION ITERNAL FUNCTION - CPU */
/***********************************/
void Jacobi_Iterator_CPU_internal(float * __restrict T, float * __restrict T_new, const int NX, const int NY)
{
int i,j;

        // --- Righe dalla 2 alla penultima
        for(j=2; j<NY-2; j++)
            for(i=1; i<NX-1; i++) {
                float T_E = T[(i+1) + NX*j];
                float T_W = T[(i-1) + NX*j];
                float T_N = T[i + NX*(j+1)];
                float T_S = T[i + NX*(j-1)];
                T_new[NX*j + i] = 0.25*(T_E + T_W + T_N + T_S);
            }
}

/***********************************/
/* JACOBI ITERATION ITERNAL FUNCTION - CPU */
/***********************************/
void Jacobi_Iterator_CPU_external(float * __restrict T, float * __restrict T_new, const int NX, const int NY)
{
int i,j;

            j=1;  // prima riga
            for(i=1; i<NX-1; i++) {
                float T_E = T[(i+1) + NX*j];
                float T_W = T[(i-1) + NX*j];
                float T_N = T[i + NX*(j+1)];
                float T_S = T[i + NX*(j-1)];
                T_new[NX*j + i] = 0.25*(T_E + T_W + T_N + T_S);
            }

            j=NY-2;  // ultima riga
            for(i=1; i<NX-1; i++) {
                float T_E = T[(i+1) + NX*j];
                float T_W = T[(i-1) + NX*j];
                float T_N = T[i + NX*(j+1)];
                float T_S = T[i + NX*(j-1)];
                T_new[NX*j + i] = 0.25*(T_E + T_W + T_N + T_S);
            }
}

/********************************/
/* TEMPERATURE INITIALIZATION : */
/* parte centrale della griglia */
/********************************/
void Init_center(float * __restrict h_T, const int NX, const int NY)
{
    int i,j;
    int startx=NX/2-NX/10;
    int endx=NX/2+NX/10;
    int starty=NY/2-NY/10;
    int endy=NY/2+NY/10;
//    int starty=NY/4;
//    int endy=NY-NY/4;
        for(j=starty; j<endy; j++)
	    for(i=startx; i<endx; i++)
              h_T[NX*j + i] = 1.0;
}


/********************************/
/* TEMPERATURE INITIALIZATION : */
/* bordo sinistro               */
/********************************/
void Init_left(float * __restrict h_T, const int NX, const int NY)
{
    int i,j;
    int startx=1;
    int endx=2;
    int starty=0;
    int endy=NY-1;
        for(j=starty; j<endy; j++)
           for(i=startx; i<endx; i++)
              h_T[NX*j + i] = 1.0;
}

/********************************/
/* TEMPERATURE INITIALIZATION : */
/* bordo alto                   */
/********************************/
void Init_top(float * __restrict h_T, const int NX, const int NY)
{
    int i,j;
    int startx=0;
    int endx=NX-1;
    for(i=startx; i<endx; i++)
              h_T[NX + i] = 1.0;
}

/********************************/
/* Periodic boundary conditions */
/* COPY BORDER: COLS            */
/********************************/
void copy_cols (float * __restrict h_T, const int NX, const int NY)
{

int i;

// copy cols
  for (i = 1; i < NY-1; ++i) {
    h_T[NX*i+0]    = h_T[NX*i+NX-2];
    h_T[NX*i+NX-1] = h_T[NX*i+1];
  }
}


/********************************/
/* Periodic boundary conditions */
/* COPY BOREDER: ROWS           */
/********************************/
void copy_rows (float * __restrict h_T, const int NX, const int NY)
{

   memcpy(&(h_T[NX*0])      ,&(h_T[NX*(NY-2)]), NX*sizeof(float) );
   memcpy(&(h_T[NX*(NY-1)]) ,&(h_T[NX*1]),      NX*sizeof(float) );
}


/******************************************/
/* print heatmap                          */
/******************************************/

void print_heatmap(float * __restrict h_T, const int NX, const int NY)
{
   int i,j;

   for (j=1; j<NY; j++){
        for (i=1; i<NX; i++) {
            printf("%.2f ",h_T[NX*j + i]);
        }
        printf("\n");
        }
}

/******************************************/
/* save heatmap  to file                  */
/******************************************/

void save_heatmap(float * __restrict h_T,  const int NX, const int NY)
{
   int i,j;
    FILE *fp;
    fp=fopen("heatmap.txt","wb");

   for (j=1; j<NY; j++) {
        for (i=1; i<NX; i++)
            fprintf(fp,"%.2f ",h_T[NX*j + i]);
          fprintf(fp,"\n");
        }

   fclose (fp);


   return;
}

/******************************************/
/* Load heatmap  from file                */
/******************************************/

void load_heatmap(float * __restrict h_T,  const int NX, const int NY)
{
   int i,j;
    FILE *fp;
    char temp[10];


   if( fp=fopen("heatmap.txt","r+")) 
    {
    for (j=1; j<NY; j++)
      {
        for (i=1; i<NX; i++)
           { if (fscanf(fp, "%s ",temp)) h_T[NX*j+i] = atof(temp); }
     }
    fclose (fp);
    printf("# File heatmap.txt loaded \n"); 
    }
   else
    printf("# File heatmap.txt doesn't exists\n"); 
  
   return;
}

/******************************************/
/* options management                     */
/******************************************/
void options(int argc, char * argv[]) {

  int i;
   while ( (i = getopt(argc, argv, "c:r:s:dh")) != -1) {
        switch (i) {
        case 'c':  WNX       = strtol(optarg, NULL, 10);  break;
        case 'r':  WNY       = strtol(optarg, NULL, 10);  break;
        case 's':  MAX_ITER  = strtol(optarg, NULL, 10);  break;
        case 'd':  debug=1; break;
        case 'h':  help=1;   break;
        default:   help=1;  break;
        }
     }
 
  }

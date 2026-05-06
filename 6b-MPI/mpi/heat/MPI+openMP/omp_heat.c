/*
omp_heat.c

module load gcc         # carica ultima versione di gcc 
gcc -O2  omp_heat.c -fopenmp -o omp_heat

./omp_heat -h
./omp_heat -t 8  > omp_heatmap.txt
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h> 
#include <omp.h>

#define BILLION  1000000000L;


void options(int argc, char * argv[]) ;
void Jacobi_Iterator_CPU(float * __restrict T, float * __restrict T_new, const int NX, const int NY); 
void Init_center(float * __restrict h_T, const int NX, const int NY); // center 
void Init_left(float * __restrict h_T, const int NX, const int NY);   // left border
void Init_top(float * __restrict h_T, const int NX, const int NY);    // top border
void copy_rows(float * __restrict h_T, const int NX, const int NY);   // periodic boundary conditions
void copy_cols(float * __restrict h_T, const int NX, const int NY);   // periodic boundary conditions
void print_colormap(float * __restrict h_T);                          // 

int NX = 512;         // --- Number of discretization points along the x axis
int NY = 512;         // --- Number of discretization points along the y axis
int MAX_ITER = 1000;  // --- Number of Jacobi iterations
int NT = 0;           // --- Number of threads
int debug=0;

/********/
/* MAIN */
/********/
int main(int argc, char **argv)
{

  int iter,i,j,result;
  char hostname[40];

  gethostname(hostname, 40);

  struct timespec start, stop; 
  double Tstart, Tstop, Tj1, Tj2;


  options(argc, argv);         /* optarg management */
  if (NT > 0)  omp_set_num_threads(NT); 


  float *h_T_new          = (float *)calloc(NX * NY, sizeof(float));
  float *h_T_old          = (float *)calloc(NX * NY, sizeof(float));
  float *h_T_temp;
  
  Tj2=0.0; 

  Tstart= omp_get_wtime();

  for(iter=0; iter<MAX_ITER; iter=iter+1)
    {

    h_T_temp=h_T_new;  // SWAP
    h_T_new=h_T_old;
    h_T_old=h_T_temp;

//    Init_center(h_T_old,  NX, NY);
//    Init_left(h_T_old,    NX, NY);
    Init_top(h_T_old,     NX, NY);
//    copy_rows(h_T_old, NX, NY);
    copy_cols(h_T_old, NX, NY);

    Tj1= omp_get_wtime();
    Jacobi_Iterator_CPU(h_T_old, h_T_new, NX, NY);
    Tj2 += omp_get_wtime()-Tj1;

    if (debug) { result=system("clear"); print_colormap(h_T_old); sleep(1);  }

    }

  Tstop= omp_get_wtime();

  fprintf (stderr,"OMP, %d, %d, %d, %d, %.3f, %.3f, %s \n",  omp_get_max_threads(), NX, NY, MAX_ITER, Tstop-Tstart , Tj2, hostname);

  if (! debug) print_colormap(h_T_old);

  free(h_T_new);
  free(h_T_old);

  return 0;
}


/***********************************/
/* JACOBI ITERATION FUNCTION - CPU */
/***********************************/
void Jacobi_Iterator_CPU(float * __restrict T, float * __restrict T_new, const int NX, const int NY)
{
int i,j; 

        // --- Only update "interior" (not boundary) node points
#pragma omp parallel  for  private (i)
        for(j=1; j<NY-1; j++)
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
#pragma omp parallel  for  private (j)
    for(i=startx; i<endx; i++)
        for(j=starty; j<endy; j++)
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
    for(i=startx; i<endx; i++)
        for(j=starty; j<endy; j++)
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
/* print color map                        */
/******************************************/

void print_colormap(float * __restrict h_T)
{
   int i,j;

   for (j=1; j<NY-1; j++){
        for (i=1; i<NX-1; i++) {
            printf("%2.2f ",h_T[NX*j + i]);
        }
        printf("\n");
        }
}

/******************************************/
/* options management                     */
/******************************************/
void options(int argc, char * argv[]) {

  int i;
   while ( (i = getopt(argc, argv, "t:c:r:s:hd")) != -1) {
        switch (i) {
        case 'c':  NX       = strtol(optarg, NULL, 10);  break;
        case 'r':  NY       = strtol(optarg, NULL, 10);  break;
        case 's':  MAX_ITER = strtol(optarg, NULL, 10);  break;
        case 't':  NT = strtol(optarg, NULL, 10);      break;
        case 'd':  debug=1; break;
        case 'h':  printf ("\n%s [-c ncols] [-r nrows] [-s nsteps] [-t threads] [-d] [-h] \n",argv[0]); exit(1);
        default:   exit(1);
        }
    }
}

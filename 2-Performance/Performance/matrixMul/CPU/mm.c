/******************************************************************************
* FILE: ser_mm.c
* DESCRIPTION:  
*   Serial Matrix Multiply - C Version
* AUTHOR: Blaise Barney
* LAST REVISED: 04/12/05

gcc -O2   mm_gsl.c  -lgsl

#se necessario installare libgsl
#apt-get install libgsl-dev

******************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>      // clock_gettime
#include <unistd.h>    // optarg
#include <string.h>    // strcpy
#include <gsl/gsl_blas.h>


void options(int argc, char * argv[]);

int n=1024;    // dimensione lato matrice quadrata
int verbose=0;
int gsl=0;     // se gsl=1 Utilizza la funzione ottimizzata della libreria GSL

char comment[100]="no-comment";
char hostname[100];

#define BILLION  1000000000L;

struct timespec start, stop;
double tempo;

int main(int argc, char *argv[])
{

options(argc, argv);  /* optarg management */

gethostname(hostname, 100);

int NCB, NRA, NCA;
NCB=n; NRA=n; NCA=n;
float gflop=(float)n*n*n*2/BILLION; 

float *a,*b,*c;

a=(float *)malloc(sizeof(float)*NRA*NCA);
b=(float *)malloc(sizeof(float)*NCA*NCB);
c=(float *)malloc(sizeof(float)*NRA*NCB);

if (verbose) printf("# Starting serial matrix multiple example...\n");
if (verbose) printf("# Using matrix sizes a[%d][%d], b[%d][%d], c[%d][%d]\n",
        NRA,NCA,NCA,NCB,NRA,NCB);

int    i, j, k;                 /* misc */

/* Initialize A, B, and C matrices */
if (verbose) printf("# Initializing matrices...\n");

   for (i=0; i<NRA; i++)
      for (j=0; j<NCA; j++)
          *(a+i*NCA+j)= i+j;
   for (i=0; i<NCA; i++)
      for (j=0; j<NCB; j++)
          *(b+i*NCA+j)= i*j;
   for(i=0;i<NRA;i++)
      for(j=0;j<NCB;j++)
         *(c+i*NCA+j)= 0.0;

/* Perform matrix multiply */
if (verbose) printf("# Performing matrix multiply...\n");

  clock_gettime( CLOCK_REALTIME  , &start);

if (gsl)  // GSL

 {
  gsl_matrix_float_view A = gsl_matrix_float_view_array(a, NRA, NCA);
  gsl_matrix_float_view B = gsl_matrix_float_view_array(b, NCA, NCB);
  gsl_matrix_float_view C = gsl_matrix_float_view_array(c, NRA, NCB);
  gsl_blas_sgemm( CblasTrans, CblasNoTrans,  1.0, &A.matrix, &B.matrix, 0.0, &C.matrix);   // float
//  gsl_blas_dgemm( CblasTrans, CblasNoTrans,  1.0, &A.matrix, &B.matrix, 0.0, &C.matrix); // double 
 }
else
 {
   for(i=0;i<NRA;i++)
      for(j=0;j<NCB;j++)
         for(k=0;k<NCA;k++)
            *(c+i*NCB+j)+= *(a+i*NCB+k) * *(b+k+j*NRA);
 }

 clock_gettime(  CLOCK_REALTIME , &stop);

 tempo = (double) ( stop.tv_sec - start.tv_sec )
          + (double) ( stop.tv_nsec - start.tv_nsec ) / BILLION;

if (verbose) 
 {
 printf("Here is the result matrix:");
 for (i=0; i<NRA; i++) { 
   printf("\n"); 
   for (j=0; j<NCB; j++) 
      printf("%6.2f   ",  *(c+i*NCB+j));
   }
  printf("\n");
 }

printf("# N  gflop  tempo(s)  GFLOP/s  Hostname   Comment  \n");
printf("%d, %.2f,  %.2f,   %.2f,  %s, %s \n", n, gflop, tempo, gflop/tempo, hostname, comment); 
   
if (verbose) printf ("\n#Done.\n");
}


void options(int argc, char * argv[])
{
  int i;
   while ( (i = getopt(argc, argv, "n:vghc:")) != -1) {
        switch (i)
        {
        case 'n':  n = strtol(optarg, NULL, 10);  break;
        case 'c':  strcpy(comment,optarg);  break;
        case 'v':  verbose = 1 ; break;
        case 'g':  gsl = 1     ; break;
        case 'h':  printf ("\n  [-n <dim_lato_matrice> ] [-c <comment>] [-v] [-h] [-g] \n "); exit(1);
        default:   exit(1);
        }
    }
}


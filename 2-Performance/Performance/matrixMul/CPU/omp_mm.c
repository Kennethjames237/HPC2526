/******************************************************************************
* FILE: mm.c
* DESCRIPTION:  
*   Serial Matrix Multiply - C Version 
* AUTHOR: Blaise Barney
* LAST REVISED: 04/12/05

gcc -fopenmp omp_mm.c -o omp_mm

******************************************************************************/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>      // clock_gettime
#include <unistd.h>    // optarg
#include <string.h>    // strcpy


void options(int argc, char * argv[]);

int n=1024;   // dimensione lato matrice quadrata
int verbose=0;   

char comment[100]="no-comment";
char hostname[100];

#define BILLION  1000000000L;

double start, stop;
int nt; 

int main(int argc, char *argv[])
{
nt=omp_get_max_threads();   /// openMP
options(argc, argv);  /* optarg management */
omp_set_num_threads(nt);    /// openMP

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

int tid, nthreads,  chunk=10; 

/* Initialize A, B, and C matrices */
if (verbose) printf("# Initializing matrices...\n");

#pragma omp parallel shared(a,b,c,nt,chunk) private(tid,i,j,k)
  {
  tid = omp_get_thread_num();
  if (tid == 0)
    {
    nthreads = omp_get_num_threads();
    if (verbose) printf("Starting matrix multiple example with %d threads\n",nthreads);
    if (verbose) printf("Initializing matrices...\n");
    }


 #pragma omp for schedule (static, chunk)
   for (i=0; i<NRA; i++)
      for (j=0; j<NCA; j++)
          *(a+i*NCA+j)= i+j;
 #pragma omp for schedule (static, chunk)	  
   for (i=0; i<NCA; i++)
      for (j=0; j<NCB; j++)
          *(b+i*NCA+j)= i*j;
 #pragma omp for schedule (static, chunk) 
   for(i=0;i<NRA;i++)
      for(j=0;j<NCB;j++)
         *(c+i*NCA+j)= 0.0;
	 
  start = omp_get_wtime(); 
/* Perform matrix multiply */
 if (verbose) printf("Thread %d starting matrix multiply...\n",tid);
 #pragma omp for schedule (static, chunk)
   for(i=0;i<NRA;i++)
      for(j=0;j<NCB;j++)
         for(k=0;k<NCA;k++)
            *(c+i*NCB+j)+= *(a+i*NCB+k) * *(b+k+j*NRA);
 stop = omp_get_wtime(); 
 }

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

 printf("#N  gflop  time(s)  GFLOPS   Hostname   Comment nthreads \n");
 printf(" %d,  %.2f,  %.2f,   %.2f, %s, %s,  %d\n", \
     n, gflop, stop-start, gflop/(stop-start),  hostname, comment, nthreads); 

 if (verbose) printf ("\n#Done.\n");
 }


void options(int argc, char * argv[])
{
  int i;
   while ( (i = getopt(argc, argv, "n:t:vhc:")) != -1) {
        switch (i)
        {
        case 'n':  n  = strtol(optarg, NULL, 10);  break;
		case 't':  nt = strtol(optarg, NULL, 10);  break;
        case 'c':  strcpy(comment,optarg);  break;
        case 'v':  verbose = 1 ; break;
        case 'h':  printf ("\n  [-n <lato_matrice> ] [-c <comment>] [-t <threads_num>] [-v] [-h] \n "); exit(1);
        default:   exit(1);
        }
    }
}


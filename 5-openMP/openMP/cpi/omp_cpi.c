//  gcc -fopenmp -O2 omp_cpi.c -o omp_cpi
//  Da parallelizzare con opemMP la funzione f2()
//

#include <stdio.h>
#include <math.h>
#include <unistd.h>    //optarg
#include <time.h>      //gettime
#include <stdlib.h>
#include <omp.h>

void options(int argc, char * argv[]) ;
double f2 (long int);

long int n=1000000000;    // intervalli
double h;
char hostname[100];

#define BILLION  1000000000L;

 double ta, tb; 
 double tp1; 
 double sum1;
 double pi1;
 int nt, tid;  ///// openMP


/********************************************/

int main( int argc, char *argv[])
{
    double  PI = 3.14159265358979323846264338327950288 ;
    gethostname(hostname, 100); 

    nt=omp_get_max_threads();   ///// openMP

    options(argc, argv);  /* optarg management */ 

    omp_set_num_threads(nt);  /// openMP

    h = 1.0 / (double) n;

    ta=omp_get_wtime();
    sum1 = f2(n); 
    tb=omp_get_wtime();

   pi1 = 4 * h * sum1;

   fprintf(stderr,"#Inter       error        time        thread        hostname \n");
   fprintf(stdout," %ld,  %.5e,  %.5f,  %d,  %s \n",
                n,  fabs(pi1-PI), tb-ta,  nt,   hostname);

    return 0;
}


double f2 ( long int n)
 {
    long int i;
    double x, sum=0.0;
    for (i = 1; i <= n; i++)
     {
       x = h * ((double)i - 0.5);
       sum += (1.0 / (1.0 + x*x));
     }
     return sum;
 }



/************************************************/

void options(int argc, char * argv[]) 
{
  int i;
   while ( (i = getopt(argc, argv, "n:t:h")) != -1) {
        switch (i) 
        {
        case 't':  nt = strtol(optarg, NULL, 10);  break;
        case 'n':  n  = strtol(optarg, NULL, 10);  break;
        case 'h':  printf ("\n%s [-n intervals] [-t threads]  [-h]\n",argv[0]); exit(1);
        default:    exit(1);
        }
    }
}


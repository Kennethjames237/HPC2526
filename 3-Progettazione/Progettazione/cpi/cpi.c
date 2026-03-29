#include <stdio.h>
#include <math.h>
#include <unistd.h>    //optarg
#include <time.h>      //gettime
#include <stdlib.h>

void options(int argc, char * argv[]) ;
double f1 (long int);
double f2 (long int);

long int n=1000000000;    // intervalli
double h; 
char hostname[100];

#define BILLION  1000000000L;

 struct timespec ta,tb;
 double tp; 
 double sum;
 double pi;

/********************************************/

int main( int argc, char *argv[])
{

    double  PI = 3.14159265358979323846264338327950288 ;

    gethostname(hostname, 100); 

    options(argc, argv);  /* optarg management */ 

    h = 1.0 / (double) n;


   clock_gettime( CLOCK_REALTIME  , &ta); ////////////////////////////////

//    sum= f1(n); 
    sum= f2(n); 

   clock_gettime( CLOCK_REALTIME , &tb); ////////////////////////////////

   pi = 4 * h * sum;

   tp = (double) ( tb.tv_sec  - ta.tv_sec )
       + (double) ( tb.tv_nsec - ta.tv_nsec )/ BILLION;


    fprintf(stderr,"#Inter       error       time         hostname \n");
    fprintf(stdout," %ld,  %.8e,  %.5f, %s \n",
                n,  fabs(pi-PI), tp, hostname);

    return 0;
}

double f1 (long int n )
 { 
    long int i; 
    double x, sum=0.0; 
    for (i = 1; i <= n; i++)
     {
       x = h * ((double)i - 0.5);
       sum += sqrt(1-x*x) ;
     }
     return sum; 
 }


double f2 (long int n )
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
   while ( (i = getopt(argc, argv, "n:s:h")) != -1) {
        switch (i) 
        {
        case 'n':  n  = strtol(optarg, NULL, 10);  break;
        case 'h':  printf ("\n%s [-n intervals] [-h]",argv[0]); exit(1);
        default:   printf ("\n%s [-n intervals] [-h]",argv[0]); exit(1);
        }
    }
}


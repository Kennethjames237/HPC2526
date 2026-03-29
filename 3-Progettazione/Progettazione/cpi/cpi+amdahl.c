// gcc cpi+amdahl.c -o cpi+amdahl -lm
//

#include <stdio.h>
#include <math.h>
#include <unistd.h>    //optarg
#include <time.h>      //gettime
#include <stdlib.h>

void options(int argc, char * argv[]) ;
double f1 (long int);
double f2 (long int);

long int n=100000000;    // intervalli
int s=100000;            // tempo usleep in microsecondi 
double h; 
char hostname[100];

#define BILLION  1000000000L;

 struct timespec ta,tb,tc;
 double tnp, tp; 
 double sum;
 double pi;

/********************************************/

int main( int argc, char *argv[])
{
   clock_gettime( CLOCK_REALTIME  , &ta); ////////////////////////////////

    double  PI = 3.14159265358979323846264338327950288 ;

    gethostname(hostname, 100); 

    options(argc, argv);  /* optarg management */ 

    h = 1.0 / (double) n;

    usleep (s);  // Simulazione codice non paralellizzabile

   clock_gettime( CLOCK_REALTIME  , &tb); ////////////////////////////////

//    sum= f1(n); 
    sum= f2(n); 

   clock_gettime( CLOCK_REALTIME , &tc); ////////////////////////////////

   pi = 4 * h * sum;

   tnp = (double) ( tb.tv_sec  - ta.tv_sec )
       + (double) ( tb.tv_nsec - ta.tv_nsec )/ BILLION;

    tp = (double) ( tc.tv_sec  - tb.tv_sec )
       + (double) ( tc.tv_nsec - tb.tv_nsec )/ BILLION;

    fprintf(stderr,"#Inter       error     tp       tnp     tp1+tnp        hostname \n");
    fprintf(stdout," %ld,  %.8e,  %.5f,  %.5f, %.5f, %s \n",
                n,  fabs(pi-PI), tp, tnp,  tnp+tp, hostname);

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
        case 's':  s  = strtol(optarg, NULL, 10);  break;
        case 'h':  printf ("\n%s [-n intervals] [-s sleep] [-h]",argv[0]); exit(1);
        default:   printf ("\n%s [-n intervals] [-s sleep] [-h]",argv[0]); exit(1);
        }
    }
}


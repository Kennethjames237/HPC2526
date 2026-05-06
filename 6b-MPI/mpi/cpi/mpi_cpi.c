//  mpicc mpi_cpi.c -o mpi_cpi  -O2
//  mpirun mpi_cpi -h
//  mpirun mpi_cpi 

#include <stdio.h>
#include <math.h>
#include <unistd.h>    //optarg
#include <time.h>      //gettime
#include <stdlib.h>
#include <mpi.h>


void options(int argc, char * argv[]) ;
double f2 (long int, long int);

double sum2local,  sum2;
double pi2;

long int n=1000000000;    // intervalli

double h; 
char hostname[100];
int  namelen;
int  mpiTasks, mpiRank;

double Ta,Tb;
int help=0;

/********************************************/

int main( int argc, char *argv[])
{

   // MPI INIT
   MPI_Init(&argc,&argv);
   MPI_Comm_size(MPI_COMM_WORLD,&mpiTasks);
   MPI_Comm_rank(MPI_COMM_WORLD,&mpiRank);
   MPI_Get_processor_name(hostname,&namelen);

   double  PI = 3.14159265358979323846264338327950288 ;

   options(argc, argv);  /* optarg management */ 

   if (help)
     {
      if (mpiRank==0) printf ("\n%s [-n intervals] [-h]\n\n",argv[0]);
      MPI_Finalize();
      return(0);
     }

   h = 1.0 / (double) n;

   long long int Lfirst=(n/mpiTasks)*mpiRank+1;
   long long int Llast=(n/mpiTasks)*mpiRank+(n/mpiTasks);

   printf("# %d/%d  %lld-%lld  %s \n", mpiRank, mpiTasks , Lfirst, Llast, hostname); 

   Ta = MPI_Wtime();
 
   sum2local = f2(Lfirst,Llast); 
   MPI_Reduce(&sum2local, &sum2, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
   pi2 = 4 * h * sum2;

   Tb = MPI_Wtime();

   if (mpiRank==0)
    fprintf(stdout,"MPI, %ld,  %.8e, %.5f,  %d,  %s \n",
              n, fabs(pi2-PI), Tb-Ta, mpiTasks,  hostname);

   MPI_Finalize();

   return 0;
}

double f2 (long int Lfirst, long int Llast )
 {
   long int i;
   double x, sum=0.0;
   for (i =  Lfirst; i <= Llast; i++)
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
   while ( (i = getopt(argc, argv, "n:h")) != -1) {
        switch (i) 
        {
        case 'n':  n  = strtol(optarg, NULL, 10);  break;
        case 'h':  help = 1; break;
        default:   help = 1; break;
        }
    }
}

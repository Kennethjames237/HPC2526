/*
 * https://linux.die.net/man/3/clock_gettime
 * This program calculates the time required to
 * execute the program specified as its first argument.
 * The time is printed in seconds, on standard out.
 * CLOCK_REALTIME              Wall-Clock time
 * CLOCK_PROCESS_CPUTIME_ID    per-process timer from the CPU
 * gcc timing.c -o timing -O2
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
 
#define BILLION  1000000000L;

int main( int argc, char **argv )
  {
    struct timespec start_realtime, stop_realtime, start_cputime, stop_cputime;
    double rt, ct;
 
   clock_gettime( CLOCK_REALTIME ,          &start_realtime) ;
   clock_gettime( CLOCK_PROCESS_CPUTIME_ID , &start_cputime) ;

   sleep (1);      //  increment real time

   int i;
   double j;
   for (i = 0; i < 500000000; i++) { j+=i*0.001; }   // increment real time  e  CPU time
   printf( "# j=%.4lf \n", j);
 
   clock_gettime( CLOCK_REALTIME           , &stop_realtime ) ;
   clock_gettime( CLOCK_PROCESS_CPUTIME_ID , &stop_cputime ) ;

    rt    = (double) ( stop_realtime.tv_sec - start_realtime.tv_sec )
            + (double) ( stop_realtime.tv_nsec - start_realtime.tv_nsec )
            / BILLION;

   ct     = (double) ( stop_cputime.tv_sec - start_cputime.tv_sec )
            + (double) ( stop_cputime.tv_nsec - start_cputime.tv_nsec )
            / BILLION;

    printf( "   timer          time\n");
    printf( "   REALTIME      %.4lf \n", rt);
    printf( "   CPUTIME       %.4lf \n", ct);
		
    return( EXIT_SUCCESS );
  }

/*
heat.c

gcc -O2 heat.c -o heat

./heat -h
./heat -s 10 -r 16 -c 16 -d
./heat -r 4096 -c 4096
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h> 

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
int debug=0;


/********/
/* MAIN */
/********/
int main(int argc, char **argv)
{

  int iter,i,j;
  int result;

  struct timespec t1, t2, t3, t4; 
  double time, Jtime;

  options(argc, argv);         /* optarg management */

  float *h_T_new          = (float *)calloc(NX * NY, sizeof(float));
  float *h_T_old          = (float *)calloc(NX * NY, sizeof(float));
  float *h_T_temp;
 
  time=0.0; 
  Jtime=0.0; 

  clock_gettime( CLOCK_REALTIME ,   &t1) ;

  for(iter=0; iter<MAX_ITER; iter=iter+1)
    {

    h_T_temp=h_T_new;  // SWAP
    h_T_new=h_T_old;
    h_T_old=h_T_temp;

    Init_center(h_T_old,  NX, NY);
    Init_left(h_T_old,    NX, NY);
    Init_top(h_T_old,     NX, NY);
    copy_rows(h_T_old, NX, NY);
    copy_cols(h_T_old, NX, NY);

    clock_gettime( CLOCK_REALTIME ,   &t2) ;
    Jacobi_Iterator_CPU(h_T_old, h_T_new, NX, NY);
    clock_gettime( CLOCK_REALTIME ,   &t3) ;

    Jtime += (double) ( t3.tv_sec -  t2.tv_sec )
          + (double) ( t3.tv_nsec - t2.tv_nsec ) / BILLION;
    
    if (debug) { result=system("clear"); print_colormap(h_T_old); sleep(1);  }

    }
   clock_gettime( CLOCK_REALTIME ,   &t4) ;

   time  = (double) ( t4.tv_sec -  t1.tv_sec )
          + (double) ( t4.tv_nsec - t1.tv_nsec ) / BILLION;

  fprintf (stderr,"%d, %d, %d, %.3f, %.3f \n", NX, NY, MAX_ITER, time, Jtime);

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
//    int starty=NY/4;
//    int endy=NY-NY/4;
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
   while ( (i = getopt(argc, argv, "c:r:s:hd")) != -1) {
        switch (i) {
        case 'c':  NX       = strtol(optarg, NULL, 10);  break;
        case 'r':  NY       = strtol(optarg, NULL, 10);  break;
        case 's':  MAX_ITER = strtol(optarg, NULL, 10);  break;
        case 'd':  debug=1; break;
        case 'h':  printf ("\n%s [-c ncols] [-r nrows] [-s nsteps] [-d] [-h]",argv[0]) ; exit(1);
        default:   exit(1);
        }
    }
}


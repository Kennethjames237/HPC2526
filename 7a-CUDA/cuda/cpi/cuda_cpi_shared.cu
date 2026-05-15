#include <stdio.h>
#include <math.h>
#include <unistd.h>    //optarg
#include <time.h>

#define BILLION  1000000000L;

void options(int argc, char * argv[]) ;


int n=500000000;    // intervalli
int nblocks=128;    // numero blocchi
int   threadsPerBlock = 1024;
const int    max_threadsPerBlock = 1024;
const double  PI = 3.14159265358979323846264338327950288 ;


__global__ void add( float *res1 ) {
	   __shared__ float shr1[max_threadsPerBlock];

       int tid = threadIdx.x + blockIdx.x * blockDim.x;       

        double h = 1.0 / (double)(gridDim.x * blockDim.x);
        double x = h * ((double)tid - 0.5);
        double pi1 = (1.0 / (1.0 + x*x)); // f1
//        double pi2 = sqrt(1-x*x); // f2

	shr1[threadIdx.x] = pi1 * 4 * h;

      __syncthreads();
    // for reductions, threadsPerBlock must be a power of 2 // because of the following code
   int i = blockDim.x/2;
   while (i != 0) {
   	 if (threadIdx.x < i)
	 shr1[threadIdx.x] += shr1[threadIdx.x + i];
	 __syncthreads();
	 i /= 2;
   }

   if (threadIdx.x==0)      res1[blockIdx.x] = shr1[threadIdx.x];
}

/************************************************/

int main(int argc, char **argv ) { 

    options(argc, argv);  /* optarg management */

    n=nblocks*threadsPerBlock; 

    float* res1=(float*)malloc(nblocks*sizeof(float));
    float *dev_res1;
    cudaMalloc( (void**)&dev_res1, nblocks*sizeof(float) );


    struct timespec t1,t2,t3,t4;
    double wtime,mtime, ktime;
    clock_gettime( CLOCK_REALTIME ,          &t1) ;

    add<<<nblocks,threadsPerBlock>>>( dev_res1);
    cudaDeviceSynchronize();

    clock_gettime( CLOCK_REALTIME ,          &t2) ;

    cudaMemcpy( res1, dev_res1, nblocks*sizeof(float), cudaMemcpyDeviceToHost ); 

    clock_gettime( CLOCK_REALTIME ,          &t3) ;

    float total1=0;
    for (int i=0;i<nblocks;i++)    total1+=res1[i];

    clock_gettime( CLOCK_REALTIME ,          &t4) ;
 
    wtime = (double) ( t3.tv_sec  - t1.tv_sec )
            + (double) ( t3.tv_nsec - t1.tv_nsec )
            / BILLION;

    mtime = (double) ( t3.tv_sec  - t2.tv_sec )
            + (double) ( t3.tv_nsec - t2.tv_nsec )
            / BILLION;

    ktime = (double) ( t2.tv_sec  - t1.tv_sec )
            + (double) ( t2.tv_nsec - t1.tv_nsec )
            / BILLION;

    fprintf(stderr,"#intervals blocks pi error wtime(s) mtime(s) ktime(s) \n");
    fprintf(stderr,"CUDA, %d, %d, %d, %.10f, %.10e, %.4f,  %.4f, %.4f \n",
          n, nblocks, threadsPerBlock, total1,  fabs(total1 - PI), wtime, mtime, ktime);

    cudaFree( dev_res1 );

    return 0; 
}

/************************************************/

void options(int argc, char * argv[])
{
  int i;
   while ( (i = getopt(argc, argv, "t:b:h")) != -1) {
        switch (i)
        {
        case 'b':  nblocks  = strtol(optarg, NULL, 10);  break;
        case 't': threadsPerBlock    = strtol(optarg, NULL, 10);  break;
        case 'h':  printf ("\n%s [-b blocks] [-h]",argv[0]); exit(1);
        default:   printf ("\n%s [-b blocks] [-h]",argv[0]);  exit(1);
        }
    }
}


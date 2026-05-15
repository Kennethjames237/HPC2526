#define DESCRIZIONE   "  \n\
Ogni thread stampa le coordinate del blocco nella gliglia \n\
e le  proprie coordinate all interno del blocco.          \n\
L'array a di N elementi viene copiato sul Device  \n\
il device copia l'array dalla memoria globale alla memoria shared  \n\n"
// quindi esegua la riduzione somma in logN iterazioni  
// Il risultato di ogni blocco si trova nel primo elemento  ( a_s[0] )
// Occore fare una riduzione di secondo livello tra i primi 
// elementi a_s[0[ di ogni blocco.

#include<stdio.h>
#define N 1024
#define BSIZE 32

int   __device__  lock=0;
float __managed__  c=0; 

__global__ void sum(float *a_d) {

__shared__ float a_s[BSIZE];  // shared 

a_s[threadIdx.x]=a_d[threadIdx.x+blockIdx.x*blockDim.x]; 
//printf("%d-%d-%.1f ", blockIdx.x, threadIdx.x, a_s[threadIdx.x]); 

  int i = BSIZE/2;
   __syncthreads();
   while (i != 0) {
         if (threadIdx.x < i)
         a_s[threadIdx.x] += a_s[threadIdx.x + i];
         __syncthreads();
         i /= 2;
   }
if (  threadIdx.x == 0 )        // sincronismo tra blocchi
   {
   printf (" %.2f ", a_s[0]);
//   atomicAdd (&c,a_s[0]);

    do {} while(atomicCAS(&lock, 0, 1));    // spin
    printf("Block %d thread %d entering critical section \n", blockIdx.x, threadIdx.x);
    c  += a_s[0];
    atomicExch(&lock, 0); // release lock
    printf("Block %d thread %d exiting critical section \n", blockIdx.x, threadIdx.x);

   }
 }


int main()
{
 float a[N]; 
 float *a_d;   

 printf(DESCRIZIONE);

 for (int i=0; i<N; i++) a[i]=1.0; 

 cudaMalloc((void**)&a_d, N * sizeof(float));  // global
 cudaMemcpy(a_d, a, sizeof(float)*N , cudaMemcpyHostToDevice); 

int GSIZE = N/BSIZE; 

 sum<<<GSIZE,BSIZE>>>(a_d);
 cudaDeviceSynchronize();

 printf("c=%.1f\n", c);

 cudaFree(a_d); 
}

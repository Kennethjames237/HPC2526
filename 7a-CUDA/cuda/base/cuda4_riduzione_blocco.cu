#define DESCRIZIONE "  \n\
L'array a di N elementi viene copiato sul Device \n\
il device copia l'array dalla memoria globale alla memoria shared \n\
quindi esegua la riduzione somma in logN iterazioni   \n\
Il risultato si trova nel primo elemento dell'array  ( a_s[0] ) \n\
e viene copiato in memoria globale (c_d)  e inviato all'host \n\n"

#include<stdio.h>
#define N     64
#define BSIZE 64
#define GSIZE 1

__global__ void sum(float *a_d, float *c_d) {

__shared__ float a_s[N];  // shared 
a_s[threadIdx.x]=a_d[threadIdx.x]; 
 
//c_d += a_s[threadIdx.x]; // Race condition!

  int i = N/2;
   __syncthreads();
   while (i != 0) {
         if (threadIdx.x < i) 
           {
               a_s[threadIdx.x] += a_s[threadIdx.x + i]; 
//               if (threadIdx.x==0) printf("%d-%.1f \n",i , a_s[threadIdx.x]);
           } 
      __syncthreads();
      i /= 2;
    }
  *c_d=a_s[0];
}


int main()
{
 printf(DESCRIZIONE);
 float a[N], c; 
 float *a_d, *c_d;   
 for (int i=0; i<N; i++) a[i]=1.0; 
 cudaMalloc((void**)&a_d, N * sizeof(float));  // global
 cudaMalloc((void**)&c_d,     sizeof(float));  // global
 cudaMemcpy(a_d, a, sizeof(float)*N , cudaMemcpyHostToDevice); 

 sum<<<GSIZE,BSIZE>>>(a_d, c_d);

 cudaMemcpy(&c, c_d, sizeof(float), cudaMemcpyDeviceToHost); 
 printf("c=%.1f\n", c);

 cudaFree(a_d); 
 cudaFree(c_d); 
}

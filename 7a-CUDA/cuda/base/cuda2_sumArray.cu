#define DESCRIZIONE   "  \n\
Gli array A B e C vengono creati sull'host e sul Device (nella memoria globale) \n\
L'host inizializza A e B e li invia al device (cudaMemcpyHostToDevice)          \n\
quindi esegue sul device il kernel sommaarray                                   \n\
infine l'array C viene trasferito da device a  host (cudaMemcpyDeviceToHost)   \n\n"

#include <stdio.h> 

__global__ void sommaarray(float *A, float *B, float *C)
{
 *(C+threadIdx.x) = *(A+threadIdx.x) + *(B+threadIdx.x);
}


int main(){ 
 float *A, *B, *C, *A_d, *B_d, *C_d; 
 int size, i, N; 

printf(DESCRIZIONE);

 N=10; 
 size=sizeof(float); 
 A=(float*)malloc(size*N); 
 B=(float*)malloc(size*N); 
 C=(float*)malloc(size*N); 
 cudaMalloc((void**)&A_d, size*N); 
 cudaMalloc((void**)&B_d, size*N); 
 cudaMalloc((void**)&C_d, size*N); 

 for(i=0; i<N; i++) { 
    *(A+i)=i+1;
    *(B+i)=i+1;
  }

 printf("Host> A: ");  
 for(i=0; i<N; i++) printf("%.1f ", A[i]);
 printf("\n");  

 printf("Host> B: ");
 for(i=0; i<N; i++) printf("%.1f ", B[i]);
 printf("\n");

 cudaMemcpy(A_d, A, size*N, cudaMemcpyHostToDevice); 
 cudaMemcpy(B_d, B, size*N, cudaMemcpyHostToDevice); 
 dim3 DimGrid(1,1); 
 dim3 DimBlock(N,1,1); 

printf ("sommaarray<<<DimGrid,DimBlock>>> \n");
sommaarray<<<DimGrid,DimBlock>>>(A_d,B_d,C_d); 
cudaMemcpy(C, C_d, size*N, cudaMemcpyDeviceToHost);

printf("Host> C: ");
for(i=0; i<N; i++) printf("%.1f ", C[i]);
printf("\n");

 cudaFree(A_d); 
 cudaFree(B_d); 
 cudaFree(C_d); 
}


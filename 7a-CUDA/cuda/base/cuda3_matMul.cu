#define DESCRIZIONE   "  \n\
Le matrici quadrate NxN A B e C vengono create sull'host e sul Device (nella memoria globale) \n\
L'host inizializza A e B e li invia al device (cudaMemcpyHostToDevice)     \n\
quindi esegue sul device il kernel matMul utilizzando un singolo blocco NxN  \n\
infine l'array C viene trasferito da device a  host (cudaMemcpyDeviceToHost) \n\n"


#include <stdio.h>

__global__ void matMul(float *A_d, float *B_d, float *C_d, int N) 
{
 int  k;
 float prod;
 prod=0;
 for(k=0; k<N; k++) {
 prod = prod + A_d[threadIdx.y*N + k] * B_d[k*N + threadIdx.x]; }
 C_d[threadIdx.y*N + threadIdx.x] = prod;
}


int main( ){ 

// __global__ void matMul(float*, float*, float*, int ); 

 float *A, *B, *C, *A_d, *B_d, *C_d;
 int size, i,j, N; 

printf(DESCRIZIONE);

N=3; 
size=sizeof(float); 
A=(float*)malloc(size*N*N); 
B=(float*)malloc(size*N*N); 
C=(float*)malloc(size*N*N); 
cudaMalloc((void**)&A_d, size*N*N); 
cudaMalloc((void**)&B_d, size*N*N); 
cudaMalloc((void**)&C_d, size*N*N); 
for(i=0; i<N*N; i++)   { 
 *(A+i)=i-1; 
 *(B+i)=i+1; 
 }

printf("\nHost> A:\n");
for(j=0; j<N; j++) 
{for(i=0; i<N; i++) printf("%.1f ", *(A+N*j+i)) ; printf("\n");}

printf("\nHost> B:\n");
for(j=0; j<N; j++)
{for(i=0; i<N; i++) printf("%.1f ", *(B+N*j+i)); printf("\n");}
printf("\n");

cudaMemcpy(A_d, A, size*N*N, cudaMemcpyHostToDevice); 
cudaMemcpy(B_d, B, size*N*N, cudaMemcpyHostToDevice); 

dim3 DimGrid(1,1); 
dim3 DimBlock(N,N,1); 	

printf ("matMul<<<DimGrid,DimBlock>>> \n");
matMul<<<DimGrid,DimBlock>>>(A_d, B_d, C_d, N); 
cudaMemcpy(C, C_d, size*N*N, cudaMemcpyDeviceToHost); 

printf("\nHost> C:\n");
for(j=0; j<N; j++)
{for(i=0; i<N; i++) printf("%.1f ", *(C+N*j+i)); printf("\n");}
printf("\n");

cudaFree(A_d); 
cudaFree(B_d); 
cudaFree(C_d); 
}



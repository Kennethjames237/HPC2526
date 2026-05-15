#define DESCRIZIONE "  \n\
Ogni thread stampa le coordinate del blocco nella gliglia \n\
e le  proprie coordinate all interno del blocco.          \n\n"

#include <stdio.h>

__global__ void print_kernel()
{
   printf("Hello from block %d/%d, thread %d/%d\n", blockIdx.x, gridDim.x, threadIdx.x, blockDim.x);
}

int main()
{
   printf(DESCRIZIONE);
   print_kernel<<<4, 8>>>();
   cudaDeviceSynchronize();
}

#define DESCRIZIONE "  \n\
Ogni thread stampa le coordinate del blocco nella gliglia                                        \n\
e le  proprie coordinate all interno del blocco.                                                 \n\
Sincronismo tra blocchi: syncthreads e' una barriera che coinvolge tutti i thread di un blocco.  \n\
Se vogliamo aggiungere un sicronismo tra i diversi blocchi creiamo una valiabile condivisa lock  \n\
in global inizializzata con il  numero di blocchi, quindi il primo thread di ogni blocco         \n\
decrementa lock in modo atomico.                                                                 \n\
Tutti i thread di tutti i blocchi attendono attivamente ( while(lock){} )  che lock sia pari a 0. \n\n"

#include <stdio.h>

__managed__ unsigned int lock;

__global__ void kernel()
{

    // do some work

    __syncthreads();

    printf("Block %d thread %d entering sync-Blocks \n ", blockIdx.x, threadIdx.x);
    if (threadIdx.x == 0)
    {
        int old;
        old = atomicSub(&lock, 1);
        printf("Block:%d-oldLock:%d \n ", blockIdx.x, old);
    }
    while (lock)
    {
    }
    printf("Block %d thread %d exiting sync-Blocks\n", blockIdx.x, threadIdx.x);
}

int main()
{

    int grid_size = 4;
    int block_size = 32;
    lock = grid_size;

    printf(DESCRIZIONE);

    kernel<<<grid_size, block_size>>>();
    cudaDeviceSynchronize();
    printf("Finito, lock:%d \n", lock);
}

#define DESCRIZIONE  " \n\
La  Unified memory (managed) consente di avere una immagine di memoria coerente    \n\
con uno spazio di indirizzamento comune tra host e device.                         \n\
Il vettore Vett, dichiarato managed, risulta visibile sia dall host che dal device \n\n"

#include <stdio.h>

#define N 32

__managed__  int  Vett[N];

__global__ void VecAdd(int *Vett)
 { 
   Vett[threadIdx.x] =  Vett[threadIdx.x] * Vett[threadIdx.x]; 
 } 


int main() {
   printf(DESCRIZIONE);
   for(int i=0; i<N; i++) Vett[i]=i; 
   VecAdd<<< 1, N >>>(Vett);  
   cudaDeviceSynchronize(); 
   for(int i=0; i<N; i++)        printf("%d: Vett*Vett = %d\n", i, Vett[i]); 
   return  0;
 }

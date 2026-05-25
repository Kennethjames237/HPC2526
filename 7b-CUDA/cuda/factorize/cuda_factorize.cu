// nvcc cuda_factorize.cu -o cuda_factorize  -lcrypto -lm

#include <stdio.h>
#include <stdint.h>
#include <cuda.h>
#include <openssl/bn.h>
#include <math.h>
#include <unistd.h>
#include <string.h>

void options(int argc, char * argv[]);
int generate_modulus (int);

int  modulus_bit=64;
int  block_dim = 32,  grid_dim=2;
unsigned long long int m=0;

__managed__ int found = 0;
__managed__ unsigned long long factor = 0;

__global__ void find_first_factor_kernel(unsigned long long M, unsigned long long max_check)
{

   unsigned long long tid = blockIdx.x * blockDim.x + threadIdx.x;
   unsigned long long stride = gridDim.x * blockDim.x;
// printf ("tid:%llu-gr:%d-bl:%d-stride:%llu ", tid, blockIdx.x, threadIdx.x, stride);

   for (unsigned long long i=tid*2+3; i<=max_check; i+=stride)
      {
        if (found) return; // stop immediato se gia¿trovato

        if (M % i == 0)
        {
            // solo il primo thread salva il fattore
           if (atomicCAS(&found, 0, 1) == 0)  factor = i;
           return;
        }
     }
}

int main( int argc, char *argv[] )
{

    options(argc, argv);
    if (!m) generate_modulus(modulus_bit);
    unsigned long long max_check=(unsigned long long) sqrt(m);

    find_first_factor_kernel<<<grid_dim, block_dim>>>(m,max_check);

    cudaDeviceSynchronize();

    if (found)   
    printf ("m=%llu grid_dim=%d block_dim=%d stride=%d max_check=%llu Factor=%llu time=", m, grid_dim, block_dim,  grid_dim*block_dim, max_check, factor);

    return 0;
}


int generate_modulus(int modulus_bit)
{
printf("generating modulus of %d bits \n", modulus_bit);

int prime_bit=(int)modulus_bit/2;

BIGNUM *P, *Q, *M;
unsigned long long int  p,q; 
P = BN_new();     // prime number
Q = BN_new();     // prime number
M = BN_new();     // modulus = p x q

BN_CTX *ctx;
ctx = BN_CTX_new();

BN_generate_prime_ex(P,prime_bit,1,NULL,NULL,NULL);
BN_generate_prime_ex(Q,prime_bit,1,NULL,NULL,NULL);

BN_mul(M,P,Q, ctx);
p = (unsigned long long) BN_get_word(P);
q = (unsigned long long) BN_get_word(Q);
m = (unsigned long long) BN_get_word(M);
printf("Modulus=%llu # %llu x %llu \n",m,p,q);

return (0);

}

/************************************************/

void options(int argc, char * argv[])
{
  int i;
   while ( (i = getopt(argc, argv, "m:t:b:g:h")) != -1) {
        switch (i)
        {
        case 'm':  m=strtol(optarg, NULL, 10);  break;
        case 't':  block_dim=strtol(optarg, NULL, 10);  break;
        case 'b':  grid_dim=strtol(optarg, NULL, 10);  break;
        case 'g':  modulus_bit=strtol(optarg, NULL, 10);  break;
        case 'h':  printf ("\n%s [-m modulus] [-t threads-per-block]  [-b blocks-per-grid] [-g generate_modulus_of_Nbit(max 64)] [-h]\n\n",argv[0]); exit(1);
        default:   printf ("\n%s [-m modulus] [-t threads-per-block]  [-b blocks-per-grid] [-g generate_modulus_of_Nbit(max 64)] [-h]\n\n",argv[0]); exit(1);
        }
    }
}


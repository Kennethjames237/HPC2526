/* 
apt install libssl-dev    
gcc -O2  factorize.c -o factorize   -lcrypto -lm 
./factorize -g 68   # genera un modulo di 68 bit
./factorize -m B81915BC0A2222F4B  -a 4    # modulus 68 bit - prime 34 bit (4 addr + 30 block) 
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/bn.h>
#include <time.h>
#include <math.h>

#define BILLION  1000000000L;

void options(int argc, char * argv[]);
void usage(char * argv[]);
int block_factorize (unsigned long int);
int generate_modulus (int);

BIGNUM  *M, *F, *ZERO,  *ONE, *TWO, *BLOCK_DIM_SIZE, *BLOCK_DIM_BIT ;
BN_CTX *ctx;
int block_idx=0, found=0, skip=0;
unsigned long int  modulus_bit, prime_bit, block_addr_bit,  block_addr_size ,  block_dim_bit , block_dim_size , block_addr_start; 
char m[256]="";  // modulus

int main(int argc, char *argv[])
{

    M = BN_new();     // module = p x q
    F = BN_new();     // found number

    ZERO           = BN_new();  //  0
    BN_set_word(ZERO,0);
    ONE            = BN_new();  //  1
    BN_set_word(ONE,1);
    TWO            = BN_new();  //  2
    BN_set_word(TWO,2);
    BLOCK_DIM_BIT  = BN_new();  // quanti bit per blocco
    BLOCK_DIM_SIZE = BN_new();  // dimensione blocco
    ctx = BN_CTX_new();
    block_addr_bit  = 8;    // default  8 bit  di address (256 sottodomini) 

    options(argc, argv);
   if (!strcmp(m,""))  strcpy(m, "B81915BC0A2222F4B");  

    modulus_bit     = strlen(m)*4;
    BN_hex2bn(&M, m);

    block_addr_size = pow(2,block_addr_bit);
    if (block_addr_start)  block_addr_size=block_addr_start+1;     // for checkpointing	
    prime_bit       = modulus_bit/2;
    block_dim_bit   = prime_bit - block_addr_bit;    // bits per block
    BN_set_word(BLOCK_DIM_BIT,block_dim_bit);            

    block_dim_size  = pow(2,block_dim_bit);           // block size

    BN_exp(BLOCK_DIM_SIZE, TWO , BLOCK_DIM_BIT,ctx);

    printf("\n# Modulus: ");
    BN_print_fp(stdout,M);


    printf(" %lu bits - prime: address %lu bit, block %lu bit\n\n", modulus_bit, block_addr_bit, block_dim_bit );
    fflush(stdout);

    struct timespec t1, t2;
    double time;

    for (block_idx=block_addr_size-1; block_idx > -1 ; block_idx--)
         {
         if (found)   continue; 
         printf("processing block %d\n",  block_idx);
         clock_gettime( CLOCK_REALTIME ,          &t1) ;
         found=block_factorize (block_idx);
         clock_gettime( CLOCK_REALTIME ,          &t2) ;
         time = (double) ( t2.tv_sec - t1.tv_sec )
                 + (double) ( t2.tv_nsec - t1.tv_nsec ) / BILLION;
         printf("processed block %d in %.1f sec. with result %d \n", block_idx, time, found);
        }
    printf("FOUND: ");
    BN_print_fp(stdout,F);
    printf("\n");
    return (0);
}


int block_factorize (unsigned long int block_addr)
{

              BIGNUM *R, *X, *Y, *BLOCK_IDX, *BLOCK_ADDR;
              BN_CTX *ctx2;
              ctx2 = BN_CTX_new();
              R       = BN_new();  // resto della divisione
              X       = BN_new();  // indice del  blocco
              Y       = BN_new();  // ultimo numero del  blocco
              BLOCK_IDX       = BN_new();  // ultimo numero del  blocco
              BLOCK_ADDR      = BN_new();  // Block Address

              BN_set_word(R,0);
              BN_set_word(X,1);
              BN_set_word(Y,1);
              BN_set_word(BLOCK_ADDR, block_addr);
              BN_mul(X, BLOCK_ADDR, BLOCK_DIM_SIZE,ctx2);   // x = i  block_size
              BN_add(X,X,ONE);                             // x = x+1 
              BN_add(Y,X,BLOCK_DIM_SIZE);                      // y = x + block_size
              BN_sub(Y,Y,TWO);                             // y = y - 2
              BLOCK_IDX=BN_dup(X);

              while ( BN_cmp(BLOCK_IDX,Y))
                {
                BN_add(BLOCK_IDX,BLOCK_IDX,TWO);
                BN_mod(R,M,BLOCK_IDX, ctx2);
                if ( BN_is_zero(R) ) 
                  {
                    F=BN_dup(BLOCK_IDX);
                    return(1);
                  }
               }
             return (0);
} 

int generate_modulus(int modulus_bit)
{
printf("generating modulus of %d bits \n",modulus_bit); 

int prime_bit;
BIGNUM *P, *Q, *M;
P = BN_new();     // prime number
Q = BN_new();     // prime number
M = BN_new();     // modulw = p x q

prime_bit=modulus_bit/2;
BN_generate_prime_ex(P,prime_bit,1,NULL,NULL,NULL);
BN_generate_prime_ex(Q,prime_bit,1,NULL,NULL,NULL);
BN_mul(M,P,Q, ctx);
printf("P=%s Q=%s \n", BN_bn2hex(P), BN_bn2hex(Q));
printf("Modulus=%s \n", BN_bn2hex(M));
return (0);

}


/************************************************/
  
void options(int argc, char * argv[])
{
  int i;
   while ( (i = getopt(argc, argv, "a:m:t:g:h")) != -1) {
        switch (i)
        {
        case 'a':  block_addr_bit   = strtol(optarg, NULL, 10);  break;
        case 'm':  strcpy(m,optarg);  break;
        case 'g':  generate_modulus(strtol(optarg, NULL, 10)); exit(1);
        case 'h':  usage(argv); exit(1);
        case '?':  usage(argv); exit(1);
        default:   usage(argv); exit(1);
        }
    }
}

/***************************************/

void usage(char * argv[])
{
  printf ("\n%s [-m modulus] [-a block_addr_bit] [-g modulus_bit] [-h]\n",argv[0]);

  printf ("\n Per generare il modulo:"); 
  printf ("\n %s -g <numero bit del modulo>",argv[0]);
  printf ("\n Esempio: %s -g 68 \n",argv[0]); 
  printf ("\n");
}

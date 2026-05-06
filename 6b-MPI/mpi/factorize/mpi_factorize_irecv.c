/*
module load intel impi
mpicc mpi_factorize_irecv.c   -lcrypto -lm  -o mpi_factorize_irecv


mpirun -n 2  mpi_factorize_irecv  -m B81915BC0A2222F4B   -a 4    # modulus 68 bit - prime 34 bit (4 addr + 28 block) 
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/bn.h>
#include <math.h>
#include <mpi.h>

//void status() {}
void options(int argc, char * argv[]) ;
int block_factorize (unsigned long int);
int generate_modulus (int);


BIGNUM *M, *F, *ZERO,  *ONE, *TWO, *BLOCK_DIM_SIZE, *BLOCK_DIM_BIT ;
BN_CTX *ctx;
int block_idx=0, block_found;
unsigned long int  modulus_bit;      // numero di bit del modulo
unsigned long int  prime_bit;        // numedo di bit del numero primo ( modulus_bit/2)
unsigned long int  block_addr_bit;   // numero di bit dell'indirizzo dei blocchi
unsigned long int  block_dim_bit;    // numero di bit di un blocco 
unsigned long int  block_addr_size;  // numero di blocchi (2^ block_addr_bit)
unsigned long int  block_dim_size;   // dimensione di un blocco (2^block_dim_bit)
unsigned long int  block_addr_start; // indirizzo del primo blocco da processare

char m[256];                         // modulus
int help=0;
int gen_modulus_bit=0;


// variables required by MPI
int MPIrank=0, MPIsize=1;
int block_addr_size_global, flag; 


int main(int argc, char *argv[])
{
        int i;
        float t1, t2, t3, t4;  // timer 
        int found=0; 

// BN setting
        M = BN_new();     // modulw = p x q
        F = BN_new();     // found number
        ZERO           = BN_new();  //  0
        ONE            = BN_new();  //  1
        TWO            = BN_new();  //  2
        BLOCK_DIM_BIT  = BN_new();  // quanti bit per blocco
        BLOCK_DIM_SIZE = BN_new();  // dimensione blocco
        ctx = BN_CTX_new();
        BN_set_word(ZERO,0);
        BN_set_word(ONE,1);
        BN_set_word(TWO,2);

// MPI init
        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &MPIrank);
        MPI_Comm_size(MPI_COMM_WORLD, &MPIsize);
        MPI_Request request;
        MPI_Status status;

// Default values
        block_addr_bit = 4;

// Options management
        options(argc, argv);

        if (!strcmp(m,""))  strcpy(m, "BD34376D359F5219");   //64 bit

        if (help)
          {
           if (MPIrank==0) printf ("\n%s [-g modulus_bit] [-s starting_block] [-a block_addr_bit] [-h]\n\n",argv[0]);
            MPI_Finalize();
            return(0);
           }

        if (gen_modulus_bit)
          {
           if (MPIrank==0) generate_modulus(gen_modulus_bit);
            MPI_Finalize();
            return(0);
           }


// Domain decomposition 
        block_addr_size_global = pow(2,block_addr_bit);
        block_addr_size = block_addr_size_global/MPIsize;

        BN_hex2bn(&M, m);
        modulus_bit     = strlen(m)*4;
        prime_bit       = modulus_bit/2;
        block_dim_bit   = prime_bit - block_addr_bit;
        block_dim_size  = pow(2,block_dim_bit);

        BN_set_word(BLOCK_DIM_BIT,block_dim_bit);                 // bits per block
        BN_exp(BLOCK_DIM_SIZE, TWO , BLOCK_DIM_BIT,ctx);

        if ( ! block_addr_start )
          block_addr_start= block_addr_size*MPIsize-1;

        if (MPIrank==0) 
         {
          printf("\n# Modulus: ");
          BN_print_fp(stdout,M);
          printf(" %lu bits,  prime %lu bit, address %lu bit, block %lu bit, start %lu \n\n", modulus_bit, prime_bit, block_addr_bit, block_dim_bit,  block_addr_start );
         }

       MPI_Barrier(MPI_COMM_WORLD); 

       MPI_Irecv(&found, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &request);

      for (block_idx=block_addr_start-MPIrank; block_idx >= 0 ; block_idx=block_idx-MPIsize)
        {
        MPI_Test(&request, &found, &status);
        if (found)  break;
        printf("task %d  processing block %d\n", MPIrank, block_idx);
        t1=MPI_Wtime();
        found=block_factorize (block_idx);
        t2=MPI_Wtime();
        if (found)
           for(i=0; i< MPIsize ; i++)
                    MPI_Send(&found, 1, MPI_INT, i , 666, MPI_COMM_WORLD);
        printf("task %d  processed block %d in %.1f sec. with result %d \n", MPIrank, block_idx, t2-t1, found);
        }

      printf("Task %d found ", MPIrank);
      BN_print_fp(stdout,F);
      printf("\n ");

      MPI_Finalize();
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
              BN_mul(X, BLOCK_ADDR, BLOCK_DIM_SIZE,ctx2);  // x = i  block_size
              BN_add(X,X,ONE);                             // x = x+1 
              BN_add(Y,X,BLOCK_DIM_SIZE);                  // y = x + block_size
              BN_sub(Y,Y,TWO);                             // y = y - 2
              BLOCK_IDX=BN_dup(X);

              while ( BN_cmp(BLOCK_IDX,Y) )
                {
                BN_add(BLOCK_IDX,BLOCK_IDX,TWO);
                BN_mod(R,M,BLOCK_IDX, ctx2);
                if ( BN_is_zero(R) ) 
                  { 
                    F=BN_dup(BLOCK_IDX);
                    return (1); // FOUND
                  }  
               }

              return (0);  // NOT FOUND
 }

/************************************************/
  
void options(int argc, char * argv[])
{
  int i;
   while ( (i = getopt(argc, argv, "s:a:b:m:g:h")) != -1) {
        switch (i)
        {
        case 's':  block_addr_start = strtol(optarg, NULL, 10);  break;
        case 'g':  gen_modulus_bit  = strtol(optarg, NULL, 10);  break;
        case 'a':  block_addr_bit   = strtol(optarg, NULL, 10);  break;
        case 'b':  modulus_bit      = strtol(optarg, NULL, 10);  break;
        case 'm':  strcpy(m,optarg);  break;
        case 'h':  help = 1; break;
        default:   help = 1; break;
        }
    }
}


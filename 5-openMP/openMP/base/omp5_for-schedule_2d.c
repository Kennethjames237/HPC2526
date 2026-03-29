#include <stdio.h>
#include <omp.h>

int main(int argc, char* argv[])
{
 int i,j; 
 int tid; // thread id 

 omp_set_num_threads(4);
        
 #pragma omp parallel  private(i,tid)
 {
   tid=omp_get_thread_num();

   #pragma omp for schedule(static,1) 
//   #pragma omp for schedule(static,1) collapse(2)
   for(j=0; j<4; j++)
    for(i=0; i<4; i++)
      printf("Esecuzione del thread %d:   j=%d i=%d \n", tid, j, i);

   printf("%d ha finito\n", tid);
 }
return 0;
}


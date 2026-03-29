#include <stdio.h>
#include <unistd.h>
#include <omp.h>

int main(int argc, char* argv[])
{
 int i, tid, chunk,  n=50,  nt=4;

 omp_set_num_threads(nt);

chunk= 1 ;              // grana fine
// chunk= (int) n/nt;    // grana grossa, default

 #pragma omp parallel  private(tid)
 {
   #pragma omp for schedule(static,chunk)
   for(i=0; i<n; i++)
      {
      tid=omp_get_thread_num();
      printf("Esecuzione del thread %d: i=%d\n", tid,i);
      }

   printf("%d ha finito\n", tid);
 }
return 0;
}


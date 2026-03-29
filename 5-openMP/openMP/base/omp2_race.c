#include <omp.h>
#include <stdio.h>

int main() {
int tid, i, j=0;

#pragma omp parallel private(tid, i) shared(j)
{
 tid = omp_get_thread_num();
 printf("running %d\n", tid);
 for (i = 0; i < 1000000; i++)
 {
// uncoment the following line
//#pragma omp critical 
 j++; /* race! */
 }
 printf("ran %d\n", tid);
}
printf("%d\n", j);
}


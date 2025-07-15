#include <stdio.h>
#include <omp.h>
int main(int argc, char *argv[]) { /* omp_parallel.c  */
#pragma omp parallel num_threads(4)
{
  int tid = omp_get_thread_num();
  printf("Alo da thread %d \n", tid);
}
return(0);
}

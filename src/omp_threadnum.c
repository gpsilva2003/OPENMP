#include <omp.h>     /* Arquivo de cabeçalho do OpenMP */
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]) { /* omp_threadnum.c  */

   omp_set_num_threads(4);
   printf("Número de threads fora da região paralela = %d\n", omp_get_num_threads());

#pragma omp parallel
{
int nthreads, tid;
/* Obtém o número da thread */
    tid = omp_get_thread_num();
    printf("Alô da thread = %d\n", tid);
/* Apenas as thread master faz isto */
    if (tid == 0) {
        nthreads = omp_get_num_threads();
        printf("Número de threads na região paralela= %d\n", nthreads);
    }
} /* Todas as threads se juntam à thread master e terminam */
    return(0);
}
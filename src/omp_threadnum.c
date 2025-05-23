#include <omp.h>     /* Arquivo de cabeçalho do OpenMP */
#include <stdio.h>

int main (int argc, char *argv[]) { /* omp_threadnum.c  */
   omp_set_num_threads(4);
   printf("Número de threads fora da região paralela = %d\n", omp_get_num_threads());
#pragma omp parallel 
{
    int tid = omp_get_thread_num(); /* Obtém o número da thread */
    printf("Alô da thread = %d\n", tid);
    #pragma omp master /* Apenas a thread master faz isto */
    {
       int nthreads = omp_get_num_threads();
       printf("Número de threads na região paralela= %d\n", nthreads);
    }
} /* Todas as threads se juntam à thread master e terminam */
    return(0);
}

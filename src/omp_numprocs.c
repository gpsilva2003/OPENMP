#include <omp.h>     /* Arquivo de cabeçalho do OpenMP */
#include <stdio.h>

int main (int argc, char *argv[]) { /* omp_numprocs.c  */
double inicio, fim;
   inicio = omp_get_wtime();
/* Imprime se estiver na região paralela */
   if (omp_in_parallel())
   	printf("Dentro da região paralela \n");
   else 
   	printf("Fora da região paralela \n");

#pragma omp parallel
{
int num_procs, max_threads, tid;
    tid = omp_get_thread_num();
/* Apenas as thread master faz isto */
    if (tid == 0) {
        num_procs = omp_get_num_procs();
        max_threads = omp_get_max_threads();
        printf("Número de processadores disponíveis = %d\n", num_procs);
/* Imprime o número máximo de threads */
        printf("Número máximo de threads = %d\n", max_threads);
/* Imprime se estiver na região paralela */
   if (omp_in_parallel())
   	printf("Dentro da região paralela \n");
   else 
   	printf("Fora da região paralela \n");
    }
} /* Todas as threads se juntam à thread master e terminam */
    fim = omp_get_wtime();
    printf("Tempo gasto na execução = %3.6f segundos. \n", fim-inicio);
    printf("Precisão da medida = %3.9f segundos\n", omp_get_wtick());
    return(0);
}

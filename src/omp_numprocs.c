#include <omp.h> 
#include <stdio.h>

int main (int argc, char *argv[]) { /* omp_numprocs.c  */
double inicio, fim;
   inicio = omp_get_wtime();
   if (!omp_in_parallel())   /* Imprime se estiver fora região paralela */
   	    printf("Fora da região paralela \n");

#pragma omp parallel num_threads(8)
{
    int tid = omp_get_thread_num();
    if (tid == 0) {   /* Apenas as thread master faz isto */
        int num_procs = omp_get_num_procs();
        int num_threads = omp_get_num_threads();
        printf("Número de processadores disponíveis = %d\n", num_procs);
        printf("Número de threads = %d\n", num_threads);  /* Imprime o número de threads */
        if (omp_in_parallel())  /* Imprime se estiver na região paralela */
   	        printf("Dentro da região paralela \n");
    }
} /* Todas as threads se juntam à thread master e terminam */
    fim = omp_get_wtime();
    printf("Tempo gasto na execução = %3.6f segundos. \n", fim-inicio);
    printf("Precisão da medida = %3.9f segundos\n", omp_get_wtick());
    return(0);
}

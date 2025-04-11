#include <stdio.h>
#include <omp.h>

int main(int argc, char *argv[]) { /* omp_for.c  */
    // Inicia uma região paralela com 4 threads
    #pragma omp parallel num_threads(4)
    // Diretiva OpenMP para dividir as iterações do laço entre as threads
    #pragma omp for 
    for (int i = 0; i < 18; ++i) 
        // Imprime o número da iteração e o ID da thread que a executou
        printf("Iteração %2d executada pela thread %d \n", i, omp_get_thread_num());

    return(0);
}
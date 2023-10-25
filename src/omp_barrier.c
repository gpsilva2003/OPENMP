#include <omp.h>
#include <stdio.h>
int main(int argc, char *argv[])   /*omp_barrier.c */
{
#pragma omp parallel num_threads(4)
{
    int tid = omp_get_thread_num();
    int num_threads = omp_get_num_threads();
    if (tid == 0 ) {
        printf ("Estou atrasado para a barreira! Tecle enter\n");
        getchar();
    }
#pragma omp barrier 
    printf("Passei da barreira. Eu sou o %d de %d processos \n", tid, num_threads);
}
    return 0;
}

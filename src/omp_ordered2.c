#include <stdio.h>
#include <omp.h>
#define N 10

int main() {
    int i;
    printf("Início do programa\n");
    // Cada iteração será paralela,
    // mas o trecho dentro de 'ordered' respeitará a ordem crescente de i.
    #pragma omp parallel for private(i) ordered num_threads(4)
    for (i = 0; i < N; i++) {
        int tid = omp_get_thread_num();
        // Trabalho paralelo (ordem não garantida)
        printf("Thread %d processando i = %d\n", tid, i);
        // Trecho crítico que deve ser exibido em ordem de i
        #pragma omp ordered
        {
            printf("Saída ordenada: i = %d, executada pela thread %d\n", i, tid);
        }
    }
    printf("Fim do programa\n");
    return 0;
}

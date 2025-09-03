#include <omp.h>
#include <stdio.h>

void compute_array(float *node, int M) {
    // Exemplo fictício de computação
    for (int j = 0; j < M; j++) {
         node[j] += 1.0f;  // Simula alguma operação
    }
}

void compute_matrix(float *array, int N, int M) {
    #pragma omp parallel
        #pragma omp single
            for (int i = 0; i < N; i++) {
                // Tarefas com diferentes prioridades 
                // (maior i --> maior prioridade)
                #pragma omp task priority(i) firstprivate(i)
                {
                    compute_array(&array[i * M], M);
                }
            }
}

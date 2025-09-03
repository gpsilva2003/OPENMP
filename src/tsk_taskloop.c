#include <omp.h>
#include <stdio.h>

// Simulações de funções externas
void long_running_task(void) {
    // Simula uma tarefa demorada
    for (volatile int i = 0; i < 100000000; ++i);
}

void loop_body(int i, int j) {
    // Simula trabalho em cada iteração
    (void)i; (void)j;
}

int main(int argc, char* argv[]) {   /* tsk_taskloop.c */
    // Exemplo 1: taskloop com grainsize e num_tasks
    int vetor[200];
    #pragma omp parallel num_threads(4)
        #pragma omp single
            #pragma omp taskloop grainsize(25) num_tasks(4)
            for (int i = 0; i < 200; ++i) {
                // Corpo da task: iterações 0-24, 25-49, etc.
                vetor[i] = i*i;
            }

    // Exemplo 2: taskloop com collapse(2)
    #pragma omp parallel num_threads(4)
        #pragma omp single
            #pragma omp taskloop collapse(2)
            for (int i = 0; i < 100; ++i)
                for (int j = 0; j < 50; ++j) {
                    // Corpo da task aninhada
                }

    // Exemplo 3: taskgroup com long task e taskloop aninhado
    int i, j;

    #pragma omp parallel
        #pragma omp single
            #pragma omp taskgroup
            {
                // Tarefa longa executada paralelamente
                #pragma omp task
                long_running_task();
                
                // taskloop paralelizado, fora do grupo (nogroup)
                #pragma omp taskloop private(j) grainsize(500) nogroup
                for (i = 0; i < 10000; ++i) {
                    for (j = 0; j < i; ++j) {
                        loop_body(i, j);
                    }
                }
            } // fim do taskgroup
    return 0;
}

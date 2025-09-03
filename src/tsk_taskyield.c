#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Simula trabalho não crítico
void algumacoisa_util(void) {
    for (volatile int i = 0; i < 10000000; i++);
}

// Simula seção crítica (exclusiva)
void algumacoisa_critica(void) {
    for (volatile int i = 0; i < 5000000; i++);
}

// Função com tarefas que tentam adquirir um lock
void foo(omp_lock_t *lock, int n) {
    for (int i = 0; i < n; i++) {
        #pragma omp task
        {
            // Executa parte útil fora da seção crítica
            algumacoisa_util();

            // Tenta adquirir o lock de forma não bloqueante
            while (!omp_test_lock(lock)) {
                // Cede a execução para outras tarefas prontas
                #pragma omp taskyield
            }

            // Seção crítica
            algumacoisa_critica();

            // Libera o lock
            omp_unset_lock(lock);
        }
    }
}

int main(int argc, char* argv[]) {  /* tsk_taskyield.c */
    omp_lock_t lock;
    omp_init_lock(&lock);
    int n = 8; // Número de tarefas
    #pragma omp parallel
    {
        #pragma omp single
        foo(&lock, n);
    }
    omp_destroy_lock(&lock);
    return 0;
}

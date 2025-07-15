#include <stdio.h>
#include <omp.h>

int main() {
    #pragma omp parallel num_threads(4)
    {
	      int tid=omp_get_thread_num();
        #pragma omp single
        {
            printf("Thread %d: Esta thread está criando tarefas para as outras threads.\n", tid);
            for (int i = 0; i < 5; i++) { // Criando múltiplas tarefas
                #pragma omp task
                {
                    int exec_id = omp_get_thread_num(); // Cada tarefa é executada por qualquer thread disponível
                    printf("Thread %d: Executando tarefa %d criada pela thread %d.\n", exec_id, i, tid);
                    for (int j = 0; j < 10000000; j++); // Simulando trabalho
                }
            }
            printf("Thread %d: Todas as tarefas foram criadas.\n", tid);
        } // Barreira implícita ao final da região single
          // Todas as threads esperam todas as tarefas serem concluídas
    }
    printf("Programa finalizado.\n");

    return 0;
}
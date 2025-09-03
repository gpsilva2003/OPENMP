#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <unistd.h>  // Para simular trabalho com sleep()

#define max_steps 5

// Definição de um tipo de árvore fictício
typedef struct {
    int dummy;
} tree_type;

// Inicializa a árvore
void init_tree(tree_type *tree) {
    tree->dummy = 0;
}

// Simula o processamento da árvore
void compute_tree(tree_type *tree) {
    printf("Computando árvore em thread %d\n", omp_get_thread_num());
    sleep(1);  // Simula tempo de processamento
    tree->dummy++;
}

// Simula uma tarefa em segundo plano
void start_background_work() {
    printf("Iniciando tarefa em segundo plano (thread %d)\n", omp_get_thread_num());
    sleep(3);  // Simula uma tarefa longa
    printf("Tarefa em segundo plano finalizada (thread %d)\n", omp_get_thread_num());
}

// Verifica se o passo foi concluído
void check_step() {
    printf("Verificando passo (thread %d)\n", omp_get_thread_num());
}

// Imprime os resultados finais
void print_results() {
    printf("Todos os resultados foram computados.\n");
}

// Função principal
int main(int argc, char* argv[]) {   /* tsk_taskgroup.c */
    int i;
    tree_type tree;
    init_tree(&tree);

    #pragma omp parallel
    {
        #pragma omp single
        {
            // Inicia a tarefa em segundo plano
            #pragma omp task
            start_background_work();

            for (i = 0; i < max_steps; i++) {
                // Garante que todas as tarefas de computação da árvore terminem antes de verificar o passo
                #pragma omp taskgroup
                    #pragma omp task firstprivate(i)
                    {
                        printf("Passo %d: início\n", i);
                        compute_tree(&tree);
                        printf("Passo %d: fim\n", i);
                    }
                check_step();
            }
        } // Aqui a thread espera a conclusão da tarefa em background
    }
    print_results();
    return 0;
}

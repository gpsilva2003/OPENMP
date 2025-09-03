#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void bar() {
    printf("bar \n")
}

void foo() { 
    int i;
    #pragma omp task if(0) // Esta tarefa é não postergável
    {
        printf("Tarefa com if(0) iniciada por thread %d\n", omp_get_thread_num());
        for (i = 0; i < 3; i++) {
            #pragma omp task // Tarefa interna regular
            {
                printf("Tarefa interna (if(0)) %d executada por thread %d\n", i, omp_get_thread_num());
                bar();
            }
        }
    }

    #pragma omp task final(1) // Tarefa regular, mas final=true
    {
        printf("Tarefa com final(1) iniciada por thread %d\n", omp_get_thread_num());
        for (i = 0; i < 3; i++) {
            #pragma omp task // Estas também serão incluídas
            {
                printf("Tarefa interna (final) - i=%d executada por thread %d\n", i, omp_get_thread_num());
                bar();
            }
        }
    }
}

int main() {  /* tsk_if_final.c */
    omp_set_num_threads(4);  // Define número de threads
    #pragma omp parallel
    {
        #pragma omp single
        {
            foo();
            #pragma omp taskwait  // Aguarda todas as tarefas
            printf("Todas as tarefas concluídas\n");
        }
    }
    return 0;
}

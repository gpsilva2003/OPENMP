#include <stdio.h>
#include <omp.h>
int main() {  /* tsk_simple.c */
    #pragma omp parallel
    {
        #pragma omp single
        {
            #pragma omp task
            {
            printf("Esta é uma tarefa executada por uma \
            thread paralela.\n");
            }
            printf("Esta é a execução fora da tarefa.\n");
        }
    }
    return 0;
}

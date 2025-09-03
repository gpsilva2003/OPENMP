#include <stdio.h>
#include <omp.h>

void foo(void) {
    int x = 2;
    // Cria uma tarefa que possivelmente compartilhará a pilha com o criador
    #pragma omp parallel
        #pragma omp single
            #pragma omp task shared(x) mergeable
            {
                x++;  //  Considerando que não há condição de corrida
            }
            #pragma omp taskwait  // Garante que a tarefa acima terminou
            printf("Valor de x = %d\n", x); // Espera-se 3
}

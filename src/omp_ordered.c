#include <stdio.h>
#include <omp.h>

void teste(int first, int last) {
    #pragma omp for schedule(static,1) ordered    // A cláusula ordered garante que as iterações sejam executadas em ordem
    for (int i = first; i <= last; ++i) {
        // Imprime as iterações ímpares
        if (i % 2)
        {
            #pragma omp ordered    // A diretiva ordered deve ser usada aqui
            printf("teste() iteração %d\n", i);
        }
    }
}

void teste2(int iter) {
   // #pragma omp ordered     // Se a diretiva for removida executa fora de ordem
    printf("teste2() iteração %d\n", iter);
}

int main(int argc, char *argv[]) { /* omp_ordered.c  */
#pragma omp parallel
 {
    teste(1, 8);             // Chama a função teste com intervalo de 1 a 8
    #pragma omp for ordered  // Imprime todas as iterações em ordem
    for (int i = 0 ; i < 5 ; i++)
        teste2(i);
 }
    return(0);
}

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char *argv[]) { /* omp_lastprivate.c  */
    int i; // Variável de controle do loop
    int x = 33; // Variável com valor inicial 33

    // A cláusula `lastprivate(x)` garante que o valor final de `x` na última iteração
    // do loop seja copiado para a variável `x` fora da região paralela
    #pragma omp parallel for lastprivate(x)
    for (i = 0; i <= 10; i++) {
        x = i; // Atualiza o valor de `x` com o índice atual do laço
        // Imprime o número da thread e o valor de `x` dentro da região paralela
        printf("Número da thread: %2d    x: %2d\n", omp_get_thread_num(), x);
    }
    // Após a região paralela, `x` conterá o valor da última iteração do laço (10)
    printf("x é %d\n", x);
    return 0;
}
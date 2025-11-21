#include <stdio.h>
#include <omp.h>
#define N 1000000

int main() {
    int i;
    long soma = 0; // variável compartilhada

    #pragma omp parallel for shared(soma) private(i)
    for (i = 0; i < N; i++)
         soma += 1;

    printf("Valor final de soma = %ld\n", soma);
    return 0;
}

#include <stdio.h>
#include <omp.h>
#define SIZE 1000
int main(int argc, char* argv[]) { /* off_if_device.c */
    int a[SIZE], b[SIZE], c[SIZE];
    // Inicia os vetores a e b no host
    for (int i = 0; i < SIZE; i++) {
        a[i] = i;
        b[i] = 2 * i;
    }
    // Offload condicional: apenas se SIZE > 500
    // e especifica o dispositivo 1 (se disponível)
    #pragma omp target if(SIZE > 500) device(1) map(to: a, b) map(from: c)
    for (int i = 0; i < SIZE; i++) {
        c[i] = a[i] + b[i];
    }
    // Verificação simples dos resultados no host
    int erro = 0;
    for (int i = 0; i < SIZE; i++) {
        if (c[i] != a[i] + b[i]) {
            erro = 1;
            printf("Erro em c[%d]: esperado %d, obtido %d\n", i, a[i] + b[i], c[i]);
            break;
        }
    }
    if (!erro)
        printf("Cálculo realizado corretamente.\n");

    return 0;
}

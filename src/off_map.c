#include <omp.h>
#include <stdio.h>

int main(int argc, char* argv[]) { /* off_map.c */
    int N = 10;
    int a[N], b[N], c[N], x[N];

    // Valor inicial dos vetores 'a' e 'b'
    for (int i = 0; i < N; i++) {
        a[i] = i + 1;
        b[i] = 2 * (i+1);
    }
    // Região paralela no dispositivo
    #pragma omp target map(to: a, b) map(from: c)
    for (int i = 0; i < N; i++) {
        c[i] = a[i] + b[i];
    }
    printf("1. c[0] = %d no hospedeiro \n", c[0]);
    // 2. Multiplica todos os elementos de 'a' por 2 e armazena em 'x',
    // com 'x' sendo alocado no dispositivo e usado em cálculos subsequentes
    #pragma omp target map(to: a) map(alloc: x)
    {
        // Inicializa o vetor 'x' no dispositivo
        for (int i = 0; i < N; i++) {
             x[i] = a[i] * 2;
        }

        // ... mais cálculos que usam 'x' podem vir aqui ...
    }
    // 3. Multiplica os primeiros N/2 elementos de 'a' por 2 e armazena em 'c'
    #pragma omp target map(to: a[0:N/2]) map(from: c[0:N/2])
    {
        for (int i = 0; i < N/2; i++) {
            c[i] = a[i] * 2;
        }
    }
    printf("2. c[0] = %d no hospedeiro \n", c[0]);
    return 0;
}

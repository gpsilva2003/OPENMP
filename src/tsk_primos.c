#include <omp.h>
#include <stdio.h>
#include <stdbool.h>

// Função para verificar se um número é primo
bool isPrime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;

    // Verificar números ímpares a partir de 5
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }

    return true;
}

int main() {
    int N = 100; // Limite superior para encontrar números primos
    int totalPrimes = 0;

    int minGrainsize = 10; // Tamanho mínimo para criar uma tarefa

    #pragma omp parallel
    {
        #pragma omp single
        #pragma omp taskloop grainsize(minGrainsize) reduction(+:totalPrimes)
        for (int i = 2; i <= N; i++) {
            if (isPrime(i)) {
                totalPrimes++;
            }
        }
    }

    printf("Total de números primos entre 1 e %d: %d\n", N, totalPrimes);

    return 0;
}

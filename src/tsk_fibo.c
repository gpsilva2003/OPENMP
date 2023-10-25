#include <omp.h>
#include <stdio.h>

// Função para calcular o n-ésimo número de Fibonacci recursivamente
int fibonacci(int n) {
    if (n <= 2) {
        return 1;
    } else {
        int x, y;
        #pragma omp task shared(x)
        x = fibonacci(n - 1);

        #pragma omp task shared(y)
        y = fibonacci(n - 2);

        #pragma omp taskwait
        return x + y;
    }
}

int main() {
    int n = 10; // Número de Fibonacci desejado

    #pragma omp parallel
    {
        #pragma omp single
        {
            int result = fibonacci(n);
            printf("O %dº número de Fibonacci é: %d\n", n, result);
        }
    }

    return 0;
}


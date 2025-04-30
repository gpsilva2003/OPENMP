#include <stdio.h>
#include <omp.h>

int fib(int n) {
    if (n < 2) {
        return n;
    } else {
        int i, j;
        #pragma omp task shared(i)
        i = fib(n - 1);
        #pragma omp task shared(j)
        j = fib(n - 2);
        #pragma omp taskwait
        return i + j;
    }
}

int main() {
    int n = 20;
    int resultado;
    #pragma omp parallel
    {
        #pragma omp single
        resultado = fib(n);
    }
    printf("O %d-ésimo número de Fibonacci é: %d\n", n, resultado);
    return 0;
}

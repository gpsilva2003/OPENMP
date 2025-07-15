#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define N 100

int main() {
    int array[N];
    int sum = 0;

    // Inicializa o array
    for (int i = 0; i < N; i++) {
        array[i] = i + 1;
    }

    #pragma omp parallel
    {
        #pragma omp single
        {
            #pragma omp task depend(out: sum)
            for (int i = 0; i < N / 2; i++) {
                sum += array[i];
            }

            #pragma omp task depend(out: sum)
            for (int i = N / 2; i < N; i++) {
                sum += array[i];
            }
        }
    }
}

#include <omp.h>
#include <stdio.h>

#define LARGE_NUMBER 10000000
double item[LARGE_NUMBER];

// Suponha que essa função execute alguma operação demorada
void process(double x) {
    // Exemplo fictício
    double r = x;
    for (int i = 0; i < 100; i++) r *= 1.000001;
}

int main(int argc, char* argv[]) {   /* tsk_untied.c */
    // Inicia o vetor (opcional)
    for (int i = 0; i < LARGE_NUMBER; i++)
        item[i] = i * 0.5;

    #pragma omp parallel
        #pragma omp single
            #pragma omp task untied
                for (int i = 0; i < LARGE_NUMBER; i++) {
                    #pragma omp task firstprivate(i)
                    {
                        process(item[i]);
                    }
                }
    return 0;
}

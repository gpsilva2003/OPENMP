#include <stdio.h>
#include <omp.h>
 
int main(int argc, char *argv[]) { /* omp_master.c  */
int a[5], i;

    #pragma omp parallel
    {
        // Realiza alguma computação
        #pragma omp for
        for (i = 0; i < 5; i++)
            a[i] = i * i;

        // Imprime resultados intermediários
        #pragma omp master
            for (i = 0; i < 5; i++)
                printf("a[%d] = %d\n", i, a[i]);

        // Espera
        #pragma omp barrier

        // Continua a computação
        #pragma omp for
        for (i = 0; i < 5; i++)
            a[i] += i;
    }
}

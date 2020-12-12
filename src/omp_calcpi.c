#include <stdio.h>
#include <omp.h>
#define N 1000000000

int main(int argc, char *argv[]) { /* omp_calcpi.c  */
double inicio, fim, pi = 0.0f; 
long i;
   inicio = omp_get_wtime();
   #pragma omp parallel for reduction(+: pi)
    for (i = 0; i < N; i++) {
        double t=(double) ((i+0.5)/N);
        pi += 4.0/(1.0+t*t);
   }
    fim = omp_get_wtime();
    printf("O valor de pi é: %f\n",pi/N);
    printf("O tempo de execução é %f segundos.\n",fim-inicio);

    return(0);
}

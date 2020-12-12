#include <stdio.h>
#include <math.h>
#include <omp.h>

double f(double x) {
double valor;
    valor = exp(x);
    return(valor);
}

int main(int argc, char *argv[]) { /* omp_trapezio.c  */
double integral;    /* Armazena resultado em integral */
double a, b;        /* Limite esquerdo e direito */
long i,n;           /* Número de Trapezóides */
double h;           /* Largura da base do Trapezóide */

    a = 0.0;
    b = 1.0;
    n = 80000000000;
    h = (b-a)/n;
    integral = (f(a) + f(b))/2.0;
    double inicio = omp_get_wtime();

#pragma omp parallel for reduction(+:integral) shared(n, h) num_threads(8)
    for (i = 1; i < n-1; i++) {
        integral += f(a + i*h);
    }
    integral *= h ;
    printf("Com n = %ld trapezoides, a estimativa \n", n);
    printf("da integral de %f ate %f = %lf \n", a, b, integral);
    printf("Tempo: \t %f \n", omp_get_wtime()-inicio);
    return(0);
}

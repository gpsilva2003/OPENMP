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
long n;             /* Número de Trapezóides */
double h;           /* Largura da base do Trapezóide */
    a = 0.0;
    b = 1.0;
    n = 8000000000;
    // retire o comentário abaixo para fazer a leitura do valor
    // scanf("%ld", &n);
    h = (b-a)/n;
    integral = (f(a) + f(b))/2.0;
    double t_inicio = omp_get_wtime();

#pragma omp parallel for default(none) firstprivate(n,a,h) reduction(+:integral) schedule(static) num_threads(4)
    for (long i = 1; i < n-1; i++) {
         integral += f(a + i*h);
    }
    integral *= h ;

    double t_fim = omp_get_wtime();
    printf("Com n = %ld trapezoides, a estimativa \n", n);
    printf("da integral de %f ate %f = %lf \n", a, b, integral);
    printf("Tempo: \t %f com %d threads.\n", t_fim-t_inicio, omp_get_max_threads());
    return(0);
}

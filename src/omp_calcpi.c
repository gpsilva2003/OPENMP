#include <stdio.h>
#include <omp.h>
const long num_passos = 10000000000;

int main(int argc, char *argv[]) { /* omp_calcpi.c  */
double passo = 1.0 / (double) num_passos;
double inicio, fim, soma = 0.0; 

   inicio = omp_get_wtime();
   #pragma omp parallel for reduction(+: soma)
   for (long int i = 0; i < num_passos; i++) {
      double x = (i + 0.5) * passo;
      soma += 4.0 / (1.0 + x * x);
   }
   double pi = soma * passo;
   fim = omp_get_wtime();
   printf("Valor calculado de Pi: %2.15f em %f segundos.\n", pi, fim-inicio);
   return(0);
}

#include <stdio.h>
#include <omp.h>
const long num_passos = 10000000000;

int main(int argc, char *argv[]) {  /* tsk_calcpi.c */
double passo = 1.0 / (double)num_passos;
double inicio, fim, soma = 0.0;

   inicio = omp_get_wtime();
   #pragma omp parallel num_threads(6) // Região paralela
   {
       int numthreads = omp_get_num_threads();
       #pragma omp single   // Só uma thread executa
       {  // Divide as iterações entre as tarefas
          #pragma omp taskloop num_tasks(numthreads) reduction(+:soma)
          for (long i = 0; i < num_passos; i++) {
               double x = (i + 0.5) * passo;
               soma += 4.0 / (1.0 + x * x);
          }
       }
   }
   double pi = passo * soma;
   fim = omp_get_wtime();
   printf("Valor aproximado de pi: %.15f Tempo de execução: %f\n", pi, fim-inici
o);
   return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>

#define N 1200000000

void sieveOfEratosthenes(long int n, char *prime) {
long int i, p, count = 0;
 #pragma omp taskloop num_tasks(8) untied
  for (i = 3; i <= n; i+=2)
    prime[i] = 1;

  prime[0] = 0;
  prime[1] = 0;
  prime[2] = 1;
  count++;

  for (p = 3; p * p <= n; p+=2)
  {
    if (prime[p] == 1)
    //#pragma omp taskloop grainsize(8000) untied
    #pragma omp taskloop num_tasks(8) untied
        for (i = p * p; i <= n; i += 2 * p)   // p*p é ímpar
            prime[i] = 0;
  }
  #pragma omp taskloop num_tasks(8) reduction(+:count) untied
  for (i = 3; i <= n; i+=2)
       if (prime[i] == 1)
           count++;
  printf("O total de números primos entre 2 e %ld é: %ld\n", n, count);
}

int main(int argc, char *argv[])
{
  long int n = N;
  char *prime = (char *)malloc((n + 1) * sizeof(char));

  for (int i = 1; i < argc; i++) {
       if (strcmp(argv[i], "-nt") == 0) {
           if (i < argc) {
               int num_threads;
               num_threads = atoi(argv[i + 1]);
               omp_set_num_threads(num_threads);
            }
       }
       else if (strcmp(argv[i], "-N") == 0)
                if (i < argc)
                    n = atoll(argv[i + 1]);
  }

  printf("O valor de N é: %ld\n", n);

  double start, end, cpu_time_used;

  start = omp_get_wtime();
  #pragma omp parallel num_threads (8)
  #pragma omp single 
  sieveOfEratosthenes(n, prime);
  end = omp_get_wtime();
  cpu_time_used = end - start;
  printf("Tempo de execução: %f segundos\n", cpu_time_used);
  free(prime);
  return 0;
}

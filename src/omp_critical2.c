#include <omp.h>
#include <stdio.h>

#define NTHREADS 4
#define N 1000
#include <omp.h>
#include <stdio.h>

void print_results(float array[N], int tid, int section) {
  int i, j;

  j = 1;
  /* sincroniza a impressão dos resultados */
  #pragma omp critical
  {
    printf("Thread %d: resultados da secção %d\n", tid, section);
    for (i = 0; i < N; i++) {
      printf("%e  ", array[i]);
      j++;
      if (j == 6) {
        printf("\n");
        j = 1;
      }
    }
  }  /* #pragma omp critical */

  #pragma omp barrier
  /* espera que todos imprimam os resultados */
  printf("Thread %d: fim resultados\n", tid);
}

int main (int argc, char *argv[]) {
  int i, nthreads, tid, section;
  float a[N], b[N], c[N];

  for (i = 0; i < N; i++)
    a[i] = b[i] = i * 1.0;

  #pragma omp parallel num_threads(NTHREADS) private(c,i,tid,section)
  {
    tid = omp_get_thread_num();
    if (tid == 0) {
      nthreads = omp_get_num_threads();
      printf("Número de threads: %d\n", nthreads);
    }
    #pragma omp barrier
    /* só continua depois da mensagem do número de threads */
    printf("Thread %d: a começar\n", tid);
    #pragma omp barrier
    /* só continua depois de todos imprimirem as mensagens iniciais */
    #pragma omp sections nowait
    {
      #pragma omp section
      {
        section = 1;
        for (i = 0; i < N; i++)
          c[i] = a[i] * b[i];
        print_results(c, tid, section);
      }
      #pragma omp section
      {
        section = 2;
        for (i = 0; i < N; i++)
          c[i] = a[i] + b[i];
        print_results(c, tid, section);
      }
    }  /* #pragma omp sections */
    #pragma omp barrier
    /* só imprime as mensagens finais quando todos terminarem */
    printf("Thread %d: a terminar\n",tid);
  }  /* #pragma omp parallel */
}



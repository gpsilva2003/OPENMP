#include <omp.h>
#include <stdio.h>

#define NTHREADS 4
#define N 1000

int main (int argc, char *argv[]) {
  int nthreads, tid, i, j;
  double a[N][N];

  #pragma omp parallel num_threads(NTHREADS) private(tid,i,j,a) shared(nthreads)
  {
    tid = omp_get_thread_num();
    if (tid == 0) {
      nthreads = omp_get_num_threads();
      printf("Número de threads: %d\n", nthreads);
    }
    #pragma omp barrier
    printf("Thread %d: a começar\n", tid);
    #pragma omp barrier
    for (i = 0; i < N; i++)
      for (j = 0; j < N; j++)
        a[i][j] = tid + i + j;
    printf("Thread %d: último %f\n", tid, a[N-1][N-1]);
    #pragma omp barrier
    printf("Thread %d: a terminar\n", tid);
  }
}

